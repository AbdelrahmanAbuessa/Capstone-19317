# BESS Monitoring System
### Capstone Project #5
### Gharbiya STEM - Team 19317 - 2024/2025

### Methods:
    The system consists of two main elements, the Electrical Element (ELEC), and the Data Management System Element (DMS). 
    Initially, The ELEC element is assembled using the ESP32 as a microcontroller connected along 2 breadboards to facilitate wiring between the ELEC components and the ESP32 (ESP). The ELEC leverages multiple sensors to achieve real-time monitoring of environmental parameters. The sensors implemented in the ELEC are the MQ-2 (MQ) gas sensor, DHT22 (DHT) humidity sensor, and DS18B20 (DS) temperature sensors. The MQ and the DHT sensors are connected directly to the ESP via GPIO pins and powered directly by the GND and VIN pins provided by the ESP. 2 DS sensors will each be powered independently by the GND and VIN pins of the ESP. They, however, are connected in series by one GPIO pin to the ESP via a data wire powered by a 4.7kΩ resistor. The connections mentioned above utilized data flow from the sensors to the ESP (Figure 1).

    Furthermore, the alarm system is a part of the ELEC and is composed of 9 LEDs and one buzzer. The buzzer is connected directly to a GPIO pin of the ESP, while the LEDs are connected in a 3x3 matrix of 3 rows and 3 columns, where the anodes of each row and the cathodes of each column are connected together by the same wire. Each row and column are connected to a GPIO pin of the ESP, and the alarm system is programmed to emit a pattern of flashing lights and sounds based on the measurements provided by the sensors.

    Additionally, the DMS element is implemented in the system by programming the ESP using Arduino IDE to connect to the nearest Wi-Fi network, collect data from the sensors, compare the data to the assigned threshold to activate the alarm system accordingly, and create an HTTP GET request containing the sensor data to send it to a google spreadsheets file. This cycle is re-iterated once every 10 seconds to minimize spreadsheet size and decrease data loss. Using Google Apps Script service, new data is placed in a new row and is organized into their corresponding cells from left to right as follows: current date, current time, temperature in Celsius, humidity percentage, and CO concentration in parts per million (ppm). Google Apps Script is also used to program the live dashboard website to extract the data from the spreadsheet and display the most recent data entry on the website. The website is also programmed to create line graphs for the latest 20 data entries to achieve data visualization effectively. The website is programmed to auto-refresh once every 10 seconds to keep up with the data updates in the spreadsheet. The live dashboard website is deployed as a web application and its link is implemented in the ESP code which is uploaded to the ESP accordingly.
    
    Furthermore, the google spreadsheets file is downloaded as a comma separated values file (.csv). A python script is created to further extract the data from the csv file and create scatter plots, calculate regression, and visualize the relations between the parameters. The script is also utilized to calculate the accuracy of the collected data by sampling the data and calculating the mean absolute error using statistical formulas mentioned further in the analysis section. All the mentioned scripts; the ESP Script, Google Apps Script, and Python Script can be found through this public GitHub repository: 19317 Capstone Code and a flowchart is created to visualize and simplify the code (Figure 2)

### You can access the Live Demo of the Website through the following link:
- https://script.google.com/macros/s/AKfycbyRwU0yobBtxFwtg1iwCkVl7aJEzueR3ni_nVmH9s4PgmRQmdxrdKk2jVSa0c0PVxQq/exec

### Figure 1: Live Dashboard
![image](https://github.com/user-attachments/assets/3c14a4f4-668f-4777-9c2e-e13bdaef9c33)

### Contributers: 
- Abdelrahman Ibrahim Aboueissa
- Eyad Saleh El-shafey
- Samy Amr Abd Elrazik
