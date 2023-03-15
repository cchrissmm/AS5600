#include <Arduino.h>
#include "AS5600.h"
#include "Wire.h"

AS5600 as5600; //  use default Wire

void serialRX()
{
  if (Serial.available())
  {
    String str = Serial.readStringUntil('\n');
        if (str.startsWith("SETUP")) 
    {
      Serial.println("Setup for Escapee 1000 requested");
      Serial.print("This chip has been burned ");
      Serial.print(as5600.getZMCO());
      Serial.println(" times before. Burn cycle starting now.");
      as5600.setMPosition(4095);
      delay(1000);
      as5600.burnAngle();
      delay(1000);
      Serial.print("New burn total (burnt ok if this number is 1 more than before");
      Serial.println(as5600.getZMCO());
    }

    if (str.startsWith("ANALOG")) // starting check for "VAR"
    {
      Serial.println("Analog100 mode requested");
      as5600.setOutputMode(AS5600_OUTMODE_ANALOG_100);
    }

    if (str.startsWith("SETMPOS")) // starting check for "VAR"
    {
      Serial.println("Set MPos mode requested: ");
      Serial.println(as5600.setMPosition(4095));
    }

    if (str.startsWith("BURNANGLE")) // starting check for "VAR"
    {
      Serial.println("Burn angle requested: ");
      as5600.burnAngle();
    }



    if (str.startsWith("STAT")) // starting check for "VAR"
    {
      Serial.print("Power Mode: 00 = Normal, 01 = LPM1, 10 = LPM2, 11 = LPM3: \t ");
      Serial.println(as5600.getPowerMode());

      Serial.print("Hysteresis 00 = off, 01 = 1 LSB, 10 = 2 LSBs, 11 = 3 LSBs:\t ");
      Serial.println(as5600.getHysteresis());

      Serial.print("Output Stage 00=analog, 01=analog limited, 10=PWM):\t ");
      Serial.println(as5600.getOutputMode(),BIN);

      Serial.print("PWM Frequency 00 = 115 Hz; 01 = 230 Hz; 10 = 460 Hz; 11 = 920 Hz:\t ");
      Serial.println(as5600.getPWMFrequency());

      Serial.print("Slow Filter 00 = 16x*; 01 = 8x; 10 = 4x; 11 = 2x:\t ");
      Serial.println(as5600.getSlowFilter());

      Serial.print("Fast Filter Threshold 000 = slow filter only, 001 = 6 LSBs, 010 = 7 LSBs, 011 = 9 LSBs,100 = 18 LSBs, 101 = 21 LSBs, 110 = 24 LSBs, 111 = 10 LSBs:\t ");
      Serial.println(as5600.getFastFilter());

      Serial.print("Watchdog Mode 0 = off, 1 = on:\t ");
      Serial.println(as5600.getWatchDog());

      Serial.print("STATUS Register:\t ");
      Serial.println(as5600.readStatus());

      Serial.print("ZMCO Status (nr of burn cycles):\t ");
      Serial.println(as5600.getZMCO());

      Serial.print("ZMCO Position:\t ");
      Serial.println(as5600.getZPosition());

      Serial.print("M Position:\t ");
      Serial.println(as5600.getMPosition());

      Serial.print("Max Angle:\t ");
      Serial.println(as5600.getMaxAngle());

      Serial.print("CONFIG:\t ");
      Serial.println(as5600.getConfigure());

      Serial.print("  GAIN:\t ");
      Serial.println(as5600.readAGC());

      Serial.print("Magnitude:\t ");
      Serial.println(as5600.readMagnitude());

      Serial.print("AGC:\t ");
      Serial.println(as5600.readAGC());

      Serial.print("DETECT:\t ");
      Serial.println(as5600.detectMagnet());

      Serial.print("M HIGH, 1= magnet too strong :\t ");
      Serial.println(as5600.magnetTooStrong());

      Serial.print("M  LOW, 1= magnet too weak:\t ");
      Serial.println(as5600.magnetTooWeak());

      Serial.print("Raw Angle:\t ");
      Serial.println(as5600.rawAngle());

      Serial.print("Angle:\t ");
      Serial.println(as5600.readAngle());

      Serial.println();
    }
  }
}



void setup()
{
  Serial.begin(115200);
  Wire.begin();
  as5600.begin(4); //  set direction pin.

  Serial.println("AS5600 burn tool");
  Serial.println("enter SETUP to start burn process");
  Serial.println("enter STATS for detailed chip info");
}

void loop()
{
  serialRX();
}
