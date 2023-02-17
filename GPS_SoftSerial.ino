#include <Wire.h>
#include <SSD1306.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

#define MYPORT_TX 14
#define MYPORT_RX 13
HardwareSerial neogps(1);
SoftwareSerial myPort;

TinyGPSPlus gps;
SSD1306Wire display(0x3c, 21, 22);

void setup()
{
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, 34, 12); ///TX RX
  myPort.begin(115200, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX, false);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  delay(10);
  
  if (!myPort)
  { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid SoftwareSerial pin configuration, check config"); 
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Invalid SoftwareSerial");
    display.drawString(0, 14, "pin configuration, check config");
    display.display();
    while (1) 
    { // Don't continue with invalid configuration
      delay (1000);
    }
  }
}

int value = 0;

String msg="Hola!...";

void loop()
{
  boolean newData = false;
    for (unsigned long start = millis(); millis() - start < 2000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
      {
        newData = true;
        
      }
    }
    
  }

  if (gps.location.isValid() == 1 && newData){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "GPS: mybustracker.ddns.net");
    display.drawString(0, 13, "Sat: "+String(gps.satellites.value()));
    display.drawString(0, 23, "Lat: "+String(gps.location.lat(),8));
    display.drawString(0, 33, "Lat: "+String(gps.location.lng(),8));
    display.drawString(0, 43, "Alt: "+String(gps.altitude.meters(),2));
    display.drawString(0, 53, "Vel: "+String(gps.speed.kmph()));
    display.display();
    myPort.println(String(gps.location.lat(),8)+","+String(gps.location.lng(),8)+","+String(gps.altitude.meters(),2)+","+String(gps.speed.kmph()));
    //delay(100);
  }
}
