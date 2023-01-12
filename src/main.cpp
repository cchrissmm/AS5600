#include <Arduino.h>
#include "AS5600.h"
#include "Wire.h"
#include "logger.h"

AS5600 as5600; //  use default Wire

#define ANALOG_PIN 14

#define THRESHOLD_LOW 1000
#define THRESHOLD_HIGH 3000

int lastValue;
int pulseCounter = 0;
int lastTime2 = 0;
int lastTime3 = 0;
int value;
int maxValue = 0;

void serialRX()
{
  if (Serial.available())
  {
    String str = Serial.readStringUntil('\n');

    if (str.startsWith("PWM920")) // starting check for "VAR"
    {
      Serial.println("PWM mode requested");
      as5600.setOutputMode(AS5600_OUTMODE_PWM);
      as5600.setPWMFrequency(AS5600_PWM_920);
    }
    if (str.startsWith("PWM115")) // starting check for "VAR"
    {
      Serial.println("PWM mode requested");
      as5600.setOutputMode(AS5600_OUTMODE_PWM);
      as5600.setPWMFrequency(AS5600_PWM_115);
    }
    if (str.startsWith("PWM460")) // starting check for "VAR"
    {
      Serial.println("PWM mode requested");
      as5600.setOutputMode(AS5600_OUTMODE_PWM);
      as5600.setPWMFrequency(AS5600_PWM_460);
    }
    if (str.startsWith("ANALOG")) // starting check for "VAR"
    {
      Serial.println("Analog100 mode requested");
      as5600.setOutputMode(AS5600_OUTMODE_ANALOG_100);
    }

    if (str.startsWith("SETMPOS")) // starting check for "VAR"
    {
      Serial.println("Set MPos mode requested: ");
      Serial.println(as5600.setMPosition(4096));
      Serial.println(as5600.getMaxAngle());
    }

    if (str.startsWith("BURNANGLE")) // starting check for "VAR"
    {
      Serial.println("Burn angle requested: ");
      as5600.burnAngle();
      Serial.println(as5600.getMaxAngle());
    }

    if (str.startsWith("STAT")) // starting check for "VAR"
    {
      Serial.print("Power Mode:\t ");
      Serial.println(as5600.getPowerMode(), HEX);

      Serial.print("Hysteresis:\t ");
      Serial.println(as5600.getHysteresis(), HEX);

      Serial.print("Output Stage:\t ");
      Serial.println(as5600.getOutputMode(), HEX);

      Serial.print("PWM Frequency:\t ");
      Serial.println(as5600.getPWMFrequency(), HEX);

      Serial.print("Slow Filter:\t ");
      Serial.println(as5600.getSlowFilter(), HEX);

      Serial.print("Fast Filter Threshold:\t ");
      Serial.println(as5600.getFastFilter(), HEX);

      Serial.print("Watchdog Mode:\t ");
      Serial.println(as5600.getWatchDog(), HEX);

      Serial.print("STATUS Register:\t ");
      Serial.println(as5600.readStatus(), HEX);

      Serial.print("ZMCO Status (nr of burn angles):\t ");
      Serial.println(as5600.getZMCO(), HEX);

      Serial.print("ZMCO Position:\t ");
      Serial.println(as5600.getZPosition(), HEX);

      Serial.print("M Position:\t ");
      Serial.println(as5600.getMPosition(), HEX);

      Serial.print("Max Angle:\t ");
      Serial.println(as5600.getMaxAngle(), HEX);

      Serial.print("CONFIG:\t ");
      Serial.println(as5600.getConfigure(), HEX);

      Serial.print("  GAIN:\t ");
      Serial.println(as5600.readAGC(), HEX);

      Serial.print("Magnitude:\t ");
      Serial.println(as5600.readMagnitude());

      Serial.print("AGC:\t ");
      Serial.println(as5600.readAGC());

      Serial.print("DETECT:\t ");
      Serial.println(as5600.detectMagnet(), HEX);

      Serial.print("M HIGH:\t ");
      Serial.println(as5600.magnetTooStrong(), HEX);

      Serial.print("M  LOW:\t ");
      Serial.println(as5600.magnetTooWeak(), HEX);

      Serial.print("Raw Angle:\t ");
      Serial.println(as5600.rawAngle());

      Serial.print("Angle:\t ");
      Serial.println(as5600.readAngle());

      Serial.println();
    }
  }
}

void countPulse()
{
  value = analogRead(ANALOG_PIN);

  //***RISING EDGE
  if (value > THRESHOLD_HIGH && lastValue < THRESHOLD_LOW)
  {
    pulseCounter++;

    // Serial.print("pulseCounter rising: ");
    // Serial.println(pulseCounter);

    // Serial.print("value: ");
    // Serial.println(value);

    // Serial.print("lastvalue: ");
    // Serial.println(lastValue);

    // Serial.print("millis: ");
    // Serial.println(millis());
  }
  //***FALLING EDGE
  if (value < THRESHOLD_LOW && lastValue > THRESHOLD_HIGH)
  {
    pulseCounter--;

    // Serial.print("pulseCounter falling: ");
    // Serial.println(pulseCounter);

    // Serial.print("value: ");
    // Serial.println(value);

    // Serial.print("lastvalue: ");
    // Serial.println(lastValue);

    // Serial.print("millis: ");
    // Serial.println(millis()); 
  }

  lastValue = value;
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  as5600.begin(4); //  set direction pin.

  pinMode(ANALOG_PIN, INPUT);
  
}

void loop()
{
  if (millis() - lastTime2 >= 1)
  {
    countPulse();
    lastTime2 = millis();
    //Serial.println(value);
  }

  if (millis() - lastTime3 >= 1000)
  {
    // Serial.print("pulseCounter: ");
    // Serial.println(pulseCounter);

    lastTime3 = millis();
  }

  serialRX();
}
