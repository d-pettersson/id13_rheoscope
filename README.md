#Rheoscope Arduino Repository

This is the repository for the ID_13 module. It consists of the microcontroller code used to run the Bright Void installation.

## Setup
To run the code, plug the physical hardware and upload the code to see the magic happening.
Once the Arduino is up and running, the green LED will light up, and the installation is good to go.

## Control
Speed of the motor is between 0-255 (it is modulated internally by the temperature and humidity values, upon the user has no control).
The direction can be influenced by sending 256 for running it forward or 257 for running it backwards.
