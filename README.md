# Parking_Lot_Sensor
By Patrick Webb, Vahagn Tovmasian

Final Project for EE128 Fall 2022 with Dr. Roman Chomko.

							
## Project Description: 
The goal of this device is to monitor multiple parking spots and to display which spots are available with LED. The LED corresponding to the nearest available parking spot will blink as an indication to the user. The project uses one way RF communication from the sensors to the main hub so that parking lot infrastructure would not have to be rebuilt, and can easily be repurposed to be used in any sort of motion detection application.

The motivation for this project is that Parking Lot sensing technology is often costly to implement depending on the shape of the parking lot, number of spots, and/or the cost of renovating the infrastructure. A camera device has a limited field of view that works best in large open spaces, however magnetic sensing requires a significant upfront investment to rebuild the lot. 

## Specification

The following project was completed using Processor Expert on the Freescale Kinetis IDE, using a K64F microcontroller. The code for the Arduino was made using the Arduino IDE 2.0, and utilises the RadioHead library and the SPI library in order to transmit and receive the wireless data, and communicate to the K64F microcontroller respectively.

## Demo Link

[Here](https://youtu.be/u2SxbELsy7A) is a link to the Youtube Video that shows the essence of the functionality.

## Schematic
![ee128](https://user-images.githubusercontent.com/19560788/205479588-659dbb60-e63a-453b-af02-a6b62484e92a.png)


## Acknowledgements

We would like to thank Dr. Roman Chomko for his well written lecture slides and notes that guided us through this project.
