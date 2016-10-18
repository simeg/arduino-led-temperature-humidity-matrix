# Arduino LED Temperature Matrix [![BuildStatus](https://travis-ci.org/simeg/arduino-led-temperature-humidity-matrix.svg?branch=master)](https://travis-ci.org/simeg/arduino-led-temperature-humidity-matrix)
A project written for the Arduino Uno R3 to get temperature and humidity data and display it on a LED matrix.

![](https://media.giphy.com/media/l0MYFmwg57Uq9FskM/giphy.gif)
## How it is used
The first four columns are used for the temperature, shown in celsius. It spans from 0C - 32C. The remaining four columns display the relative humidity which spans from 0% - 32%.

### Tested with
* DHT11 (Temperature and humidity sensor)
* LED Matrix 8x8 (MAX7219)

### Libraries used
* [DHT](https://github.com/adafruit/DHT-sensor-library)
* [LedControl](https://github.com/wayoda/LedControl)
* [HashMap](http://playground.arduino.cc/Code/HashMap)
* [StackArray](http://playground.arduino.cc/Code/StackArray)
