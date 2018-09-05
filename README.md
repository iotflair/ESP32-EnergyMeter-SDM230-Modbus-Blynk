# ESP32-EnergyMeter-SDM230-Modbus-Blynk

## About

This example Arduino based code implements procedure of reading data( Voltage,Current,Power,Frequency) from Energy Meter SDM230 and send it via ESP32 Dev Board to Blynk app.
TTL to RS485 adapter is used in that project.UART side pins Rx and Tx are connected respectively to GPIO16 and GPIO17 of ESP32 Dev Board.RS485 side - respectively to the energy meter.Used protcol for reading energy meter data is Modbus.
Used Blynk Virtual Pins are V5,V6,V7 and V8.
It needs to be installed Blynk library in Arduino program.

Image of the scheme and connections is shown at /IMG/emeter.jpg
