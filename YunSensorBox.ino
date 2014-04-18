// Copyright (C) 2014 Joshua Warren 
/* 
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
*/


#include <EmonLib.h> // https://github.com/openenergymonitor/EmonLib 
#include <OneWire.h> // http://www.pjrc.com/teensy/td_libs_OneWire.html 
#include <DallasTemperature.h> // https://github.com/milesburton/Arduino-Temperature-Control-Library

#include <Bridge.h> // included in the Arduino IDE as of ~v1.5 
#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>

#define ONE_WIRE_BUS 3  // Connect your 1-wire devices to digital pin 3

#define WATER_SENSOR A0 // water sensor on pin A0 
#define CURRENT_SENSOR_PIN A1 // current sensor on pin A1

/* 
	Replace these with the addresses you discover for your sensors. 
 */ 

DeviceAddress thermoSensorOne = { 0x28,  0xcc,  0xa4,  0x4f,  0x05,  0x00,  0x00,  0x9f };
DeviceAddress thermoSensorTwo = { 0x28, 0xE3, 0x21, 0x4f, 0x05, 0x00, 0x00, 0x14 }; 
DeviceAddress thermoSensorThree = { 0x28,  0x17,  0x54,  0x4f,  0x05,  0x00,  0x00,  0x05 }; 
DeviceAddress thermoSensorFour = { 0x28,  0x6d,  0x2c,  0x4f,  0x05,  0x00,  0x00,  0x01 }; 
DeviceAddress thermoSensorFive = { 0x28,  0xaa,  0xc1,  0x4f,  0x05,  0x00,  0x00,  0x2d }; 
DeviceAddress thermoSensorSix = { 0x28,  0xa3,  0xa8,  0x4f,  0x05,  0x00,  0x00,  0x59 }; 

EnergyMonitor emon1;

OneWire  oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT); 
  Bridge.begin();
  digitalWrite(13, HIGH); // turn on the red LED when our setup process starts
  Bridge.put("status", "startup");
  discoverOneWireDevices();
  sensors.begin();
  sensors.setResolution(thermoSensorOne, 10);
  sensors.setResolution(thermoSensorTwo, 10);
  emon1.current(CURRENT_SENSOR_PIN, 111.1);             // Current: input pin, calibration.
}

void loop() {
  // put your main code here, to run repeatedly:
  Bridge.put("status", "running");
  int waterLevelVoltage = analogRead(WATER_SENSOR);
  Bridge.put("waterLevelVoltage", String(waterLevelVoltage)); 
  sensors.requestTemperatures();
  delay(30000); // pause 30 seconds - server density only updates every 60 seconds 
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  Bridge.put("currentSensorIrms", String(Irms));
  measureTemperature(thermoSensorOne, "ACUnitInterior");	
  measureTemperature(thermoSensorTwo, "Ceiling");	// Replace 'Ceiling' with location of your sensors
  measureTemperature(thermoSensorThree, "ACVent");
  measureTemperature(thermoSensorFour, "BuildingACVent");
  measureTemperature(thermoSensorFive, "SensorBox");
  measureTemperature(thermoSensorSix, "Hallway");
}

void measureTemperature(DeviceAddress deviceAddress, String sensorName)
{
  String sensorNameC = sensorName + 'C';
  String sensorNameF = sensorName + 'F'; 
  
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00 || tempC == 11569 || tempC > 82 || tempC < 0 ) {
    // set no values if we didn't get a valid response
  } else {
    Bridge.put(sensorNameC, String(tempC));
    Bridge.put(sensorNameF, String(DallasTemperature::toFahrenheit(tempC)));
  }
}


void discoverOneWireDevices(void) {
  int x; 
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  String deviceName;
  String prettyAddr; 
  OneWire ds = oneWire;
  
  x = 0; 
  
  Bridge.put("status", "searching"); 
  while(ds.search(addr)) {
    deviceName = "Device-";
    deviceName += x;
    x++;
    prettyAddr =  "";
    
    for( i = 0; i < 8; i++) {
      prettyAddr += " 0x";    
      if (addr[i] < 16) {
        prettyAddr += 0; 
      }
      prettyAddr += String(addr[i], HEX); 
      if (i < 7) {
              prettyAddr += ", "; 
      }
    }
    Bridge.put(deviceName, prettyAddr);
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        return;
    }
  }
  ds.reset_search();
  Bridge.put("temperatureSensorsFound", String(x));
  return;
}