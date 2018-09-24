/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>
#include "SDM.h"                                                               //import SDM library

HardwareSerial SerialSDM(1);                                                   //Set Hardware Serial(pins 16 & 17)
SDM sdm(SerialSDM, 2400);                                                      //config SDM

float voltage,current,power,frequency,energy;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "AUTH";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID";
char pass[] = "PASS";

void sdm_get(void);

BlynkTimer timer;

void myTimerEvent()
{
  sdm_get();
  Blynk.virtualWrite(V5, voltage);
  Blynk.virtualWrite(V6, current);
  Blynk.virtualWrite(V7, power);
  Blynk.virtualWrite(V8, frequency);
  Blynk.virtualWrite(V9, energy);
}


void setup()
{
  // Debug console
  Serial.begin(9600);
  sdm.begin(); 

  Blynk.begin(auth, ssid, pass);

  myTimerEvent();
  // Setup a function to be called every 10 minutes
  timer.setInterval(600000L, myTimerEvent);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
      WiFi.reconnect();
      //Serial.print("Reconnecting....");
      delay(5000);
  }
  Blynk.run();
  timer.run(); // Initiates BlynkTimer 
}

//************************* SDM230  ******************************//
void sdm_get(){ 
  char bufout[10];
  sprintf(bufout, "%c[1;0H", 27);
  Serial.print(bufout);

  Serial.print("Voltage:   ");
  voltage = sdm.readVal(SDM230_VOLTAGE);
  Serial.print(voltage, 2);                                //display voltage
  Serial.println("V");

  delay(50);

  Serial.print("Current:   ");
  current = sdm.readVal(SDM230_CURRENT);
  Serial.print(current, 2);                                //display current  
  Serial.println("A");

  delay(50);

  Serial.print("Power:     ");
  power = sdm.readVal(SDM230_POWER);
  Serial.print(power, 2);                                  //display power
  Serial.println("W");

  delay(50);

  Serial.print("Frequency: ");
  frequency = sdm.readVal(SDM230_FREQUENCY);
  Serial.print(frequency, 2);                              //display frequency
  Serial.println("Hz");   
  
  delay(50);

  Serial.print("Total Active Energy: ");
  energy = sdm.readVal(SDM230_TOTAL_ACTIVE_ENERGY  );
  Serial.print(energy, 2);                                  //display total active energy
  Serial.println("kWh");   

}
//***************************************************************//
