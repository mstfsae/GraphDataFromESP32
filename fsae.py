import matplotlib
matplotlib.use('TkAgg')  # Set the backend to TkAgg

import requests
import time
import matplotlib.pyplot as plt
from bs4 import BeautifulSoup

def scrape_website(url):
    response = requests.get(url)
    if response.status_code == 200:
        soup = BeautifulSoup(response.content, 'html.parser')
        data_element = soup.find('p')
        data = data_element.text.strip()
        return data
    else:
        print("Failed to retrieve webpage.")

url = 'http://192.168.4.1/'

# Set up fixed window size for the plot
window_size = 10
num_seconds = [0] * window_size
data_values = [0] * window_size  # Initialize data_values with zeros

plt.xlabel('Time')
plt.ylabel('Integer Value')
plt.title('Time vs Integer Value')

# Set up the plot
fig, ax = plt.subplots()
line, = ax.plot(num_seconds, data_values, marker='o', linestyle='-')

# Record the start time of the program
start_time = time.time()

# Main loop to continuously update the plot

times = 0
while True:

    data = scrape_website(url)
    if data is not None:
        data_value = int(data.split(':')[1].strip())
        data_values.append(data_value)

        # Update the x-axis data to maintain the sliding window
        current_time = time.time() - start_time
        num_seconds.append(current_time)
        num_seconds = num_seconds[-window_size:]  # Truncate to fixed window size

        # Update the plot with new data and adjusted x-axis limits
        line.set_xdata(num_seconds)
        line.set_ydata(data_values[-window_size:])
        ax.set_xlim(num_seconds[0], num_seconds[-1])  # Set x-axis limits based on current data
        ax.relim()  # Update limits of the axes
        ax.autoscale_view()  # Autoscale the view
        fig.canvas.draw()  # Redraw the plot
        plt.pause(0.1)  # Adjust pause interval as needed
    else:
        print("Failed to retrieve data from the website.")

    # Finally, display the plot


plt.show()
