#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "SDM.h"                                                               //import SDM library

HardwareSerial SerialSDM(1);                                                   //Set Hardware Serial(pins 16 & 17)
SDM sdm(SerialSDM, 2400);   

float voltage,current,power,frequency,energy;

// You should get Auth Token in the Blynk App.
char auth[] = "AUTH";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID";
char pass[] = "PASS";

void sdm_get(void);
bool triggerBlynkConnect = false;
bool isFirstConnect = true; // Keep this flag not to re-sync on every reconnection
BlynkTimer timer;


// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(F("WiFi not connected"));
    
    Blynk.connectWiFi(ssid, pass);
  }
  else
  {
    Serial.println(F("WiFi in connected"));
  }

  if (!Blynk.connected() && WiFi.status() == WL_CONNECTED)
  {
    if (Blynk.connect())
    {
      Serial.println(F("Blynk reconnected"));
    }
    else
    {
      Serial.println(F("Blynk not reconnected"));
    }
  }
  else
  {
    Serial.println(F("Blynk connected"));
  }

  if (Blynk.connected() && WiFi.status() == WL_CONNECTED)
  {
    sdm_get();
    Blynk.virtualWrite(V5, voltage);
    Blynk.virtualWrite(V6, current);
    Blynk.virtualWrite(V7, power);
    Blynk.virtualWrite(V8, frequency);
    Blynk.virtualWrite(V9, energy);

  }
}

BLYNK_CONNECTED()
{
  triggerBlynkConnect = true;

  Serial.println(F("Blynk Connected!"));
  Serial.println(F("local ip"));
  Serial.println(WiFi.localIP());

  if (isFirstConnect)
  {       
    Blynk.syncAll();    

    isFirstConnect = false;
  }
}


void setup() {
  Serial.begin(9600);
  sdm.begin(); 
  
  Blynk.config(auth);
  Blynk.connectWiFi(ssid, pass);

  if (WiFi.status() == WL_CONNECTED)
  {
    if (Blynk.connect())
    {
      triggerBlynkConnect = true;
    }
    else
    {
      triggerBlynkConnect = false;
    }
  }

  myTimerEvent();
  timer.setInterval(600000L, myTimerEvent);       //Every 10 minutes
}
void loop() {

  if (Blynk.connected())
  {
    Blynk.run(); // Initiates Blynk Server  
  }
  else
  {
    triggerBlynkConnect = false;
  }
  
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
