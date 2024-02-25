#include <WiFi.h>
#include <time.h>

//Local access point credentials
const char* ssid     = "HenryMoment";
const char* password = "HenryBad";


//Set web server port number to 80
WiFiServer server(80);

//Variable to store the HTTP request
String header;

//Task handles to allow for multi core support
TaskHandle_t Task1;
TaskHandle_t Task2;

void setup()
{
  Serial.begin(115200);

 
  //Set up and start the access point
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
 
  server.begin();


  delay(500);


  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500);
}






void Task2code( void * pvParameters ){
//  Serial.print("Task2 running on core ");
//  Serial.println(xPortGetCoreID());


  for(;;){
    //Listen for incomming clients
  
  WiFiClient client = server.available();

  Serial.println(client);


  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
   
    int temperature = 130;
    int randint = 0;
    int randint2 = 0;
    randomSeed(time(NULL));
    while(client.connected()) {            // loop while the client's connected
     
      Serial.print("here");
      randint = random() % 20;
      randint2 = random() % 10 + 1;

      if (randint2 > 5){
        temperature += randint;
      }      
      else
      {
        temperature -= randint;
      }

      Serial.print(randint);
     
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
           
           
           
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
           
            //Web Page Heading
            client.println("<body><h1>Data!!</h1>");
            client.println("<h1></h1>");


            client.println("<p>Data: " + String(temperature) + "</p>");
         
            client.print("<meta http-equiv=\"refresh\" content=\".1\">");


           
            client.println("</body></html>");
           
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          {                       // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                         // if you got anything else but a carriage return character,
          currentLine += c;       // add it to the end of the currentLine
        }
      }
    }
   
    //Clear the header variable
    header = "";
   
    //Close the connection
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
  }
}

void loop()
{
 
}



