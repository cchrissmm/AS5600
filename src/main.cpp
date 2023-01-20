#include <Arduino.h>
#include "AS5600.h"
#include "Wire.h"
#include "motor.h"

AS5600 as5600; //  use default Wire

const int M01DIRPIN = 26;
const int M01PWMPIN = 27;
const int M01SIGPIN = 14;

const int PWM_FREQ = 5000;
const int PWM_RES = 12;

Motor M01(M01DIRPIN,M01PWMPIN,M01SIGPIN);

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
        if (str.startsWith("SETUP")) // starting check for "VAR"
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

    if (str.startsWith("RUNUP")) 
    {
      Serial.println("Motor Run UP requested");
      M01.runUp(255);
    }

        if (str.startsWith("RUNDOWN")) 
    {
      Serial.println("Motor Run DOWN requested");
      M01.runDown(255);
    }

        if (str.startsWith("STOP")) 
    {
      Serial.println("Motor STOP requested");
      M01.stop();
    }

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
      Serial.println(as5600.setMPosition(4095));
    }

    if (str.startsWith("BURNANGLE")) // starting check for "VAR"
    {
      Serial.println("Burn angle requested: ");
      as5600.burnAngle();
    }

    if (str.startsWith("AVALUE")) // starting check for "VAR"
    {
      Serial.println("Get Analog pin current value (mV): ");
      Serial.println(analogReadMilliVolts(M01SIGPIN));
    }

    if (str.startsWith("STAT")) // starting check for "VAR"
    {
      Serial.print("Power Mode:\t ");
      Serial.println(as5600.getPowerMode());

      Serial.print("Hysteresis:\t ");
      Serial.println(as5600.getHysteresis());

      Serial.print("Output Stage (00=analog, 01=analog limited, 10=PWM):\t ");
      Serial.println(as5600.getOutputMode(),BIN);

      Serial.print("PWM Frequency:\t ");
      Serial.println(as5600.getPWMFrequency());

      Serial.print("Slow Filter:\t ");
      Serial.println(as5600.getSlowFilter());

      Serial.print("Fast Filter Threshold:\t ");
      Serial.println(as5600.getFastFilter());

      Serial.print("Watchdog Mode (0=off):\t ");
      Serial.println(as5600.getWatchDog());

      Serial.print("STATUS Register:\t ");
      Serial.println(as5600.readStatus());

      Serial.print("ZMCO Status (nr of burn angles):\t ");
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

      Serial.print("M HIGH:\t ");
      Serial.println(as5600.magnetTooStrong());

      Serial.print("M  LOW:\t ");
      Serial.println(as5600.magnetTooWeak());

      Serial.print("Raw Angle:\t ");
      Serial.println(as5600.rawAngle());

      Serial.print("Angle:\t ");
      Serial.println(as5600.readAngle());

      Serial.print("Signal pin value (mV):\t ");
      Serial.println(analogReadMilliVolts(M01SIGPIN));

      Serial.println();
    }


  }
}

void countPulse()
{
  value = analogRead(M01SIGPIN);

  //***RISING EDGE
  if (value > THRESHOLD_HIGH && lastValue < THRESHOLD_LOW)
  {
    pulseCounter++;
  }
  //***FALLING EDGE
  if (value < THRESHOLD_LOW && lastValue > THRESHOLD_HIGH)
  {
    pulseCounter--;
  }

  lastValue = value;
}

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  as5600.begin(4); //  set direction pin.

  pinMode(M01SIGPIN, INPUT);

//LIFTER01 Setup
    ledcAttachPin(M01PWMPIN, 0);
    ledcSetup(0, PWM_FREQ, PWM_RES);
    pinMode(M01DIRPIN, OUTPUT);
    pinMode(M01SIGPIN, INPUT); // may need pullup
  
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
