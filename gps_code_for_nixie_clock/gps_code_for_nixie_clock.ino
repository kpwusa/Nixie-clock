#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample code demonstrates just about every built-in operation of TinyGPS++ (TinyGPSPlus).
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// For stats that happen every 5 seconds
unsigned long last = 0UL;
int hour_var;
void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);

  Serial.println(F("KitchenSink.ino"));
  Serial.println(F("Demonstrating nearly every feature of TinyGPS++"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void loop()
{
  // Dispatch incoming characters
  while (ss.available() > 0)
    gps.encode(ss.read());

  

 

   if (gps.time.isUpdated())
  {
    
    
    //Serial.print(F("TIME       Fix Age="));
   // Serial.print(gps.time.age());
   // Serial.print(F("A"));
   // Serial.print(gps.time.value());
   // Serial.print(F(" Hour="));
   hour_var = gps.time.hour()+7;  //time zone
  
      if (hour_var > 24)     //24hr correction
   {
   hour_var = hour_var-24; 
   }
      
      if (hour_var > 12)   //12 hr correction 
   {
   hour_var = hour_var-12; 
   }   
      
   char sz[32];
   sprintf(sz, "A%02d.%02d.%02dZ ", hour_var, gps.time.minute(), gps.time.second());
   // Serial.print(gps.time.hour());
   // Serial.print(F(" Minute="));
   // Serial.print(gps.time.minute());
    //Serial.print(F(" Second="));
   // Serial.print(gps.time.second());
   Serial.println(sz);
   // Serial.println(F("Z"));
   delay(400);
    
  }

  
  else if (millis() - last > 5000)
  {
    //Serial.println();
    if (gps.location.isValid())
    {
      
    }

   

    if (gps.charsProcessed() < 10)
      Serial.println(F("WARNING: No GPS data.  Check wiring."));

    last = millis();
    //Serial.println();
  }
}
