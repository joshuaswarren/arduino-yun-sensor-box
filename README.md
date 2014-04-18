arduino-yun-sensor-box
======================

Code to use an Arduino yun as a sensor box tied in to Server Density

## Purpose

This system allow you to obtain temperature, water-level and current readings and feed them back to [Server Density](http://www.serverdensity.com/) where they can be monitored, graphed and alerts can be fired based on the values. 

Even without a Server Density account, the Arduino Yun will provide the sensor values via JSON-encoded data on a REST API, so it should be fairly trivial to integrate this data into other platforms or your own custom application.  

This system is deployed in the [Creatuity](http://creatuity.com/) offices to allow us to monitor our Liebert AC unit that cools our server room. The current sensor is connected to one of the power connections to the condensate pump, the water level sensor is located within the pan and the temperature sensors are spread throughout - in and around the AC unit itself, the server room and other nearby areas.

The temperature sensors are 1-wire DS18B20's, meaning you can cheaply and easily deploy a large number of these sensors throughout a room or building using the same circuit. 

## Requirements

You'll need an Arduino Yun and some sensors. The Yun can be connected via Ethernet or Wifi - we connected ours to both for a bit of extra redundancy. 

The temperature sensors are DS18B20 one wire sensors.

The water sensor is a "Phantom YoYo High Sensitivity Water Sensor".

The current sensor is a "Non-Invasive Current Sensor - 30A". 

The current sensor will require a small circuit of resistors and a capacitor. See more details at [SparkFun's product page](https://www.sparkfun.com/products/11005) for the sensor. 

## Connections

Connect your one wire bus to Digital Pin 3 on the Yun. Also connect a 4.7k ohm resistor between the one wire data bus and +5V on the Yun. 

Connect your water sensor's output to analog pin 0 on the Yun.

Connect your current sensor's output to analog pin 1 on the Yun.

Power and ground should be pulled from the +5V and GND pins on the Yun. 

Connect the Yun to a microusb power supply and an Ethernet connection. 

## Running 

After making the connections, program the Yun with YunSenorBox.ino, using the Arduino IDE. 

Then, copy Yunsensorbox.py and yunsensorbox.php to your SD-agent Plugins directory, modifying the path in Yunsensorbox.py if needed.

I would recommend running yunsensorbox.php from the command line once to make sure you get valid JSON output - if you don't, the plugin is liable to crash your running SD-agent. 

Within a few minutes, the data should begin appearing in Server Density's web interface. 

## License

Copyright (C) 2014 [Joshua Warren](http://joshuawarren.com/) 
This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
