# AS5600 sensor setup

This is a helper to set up an AS5600 rotary encoder for Arduino.

Data sheet here:

https://dammedia.osram.info/media/resource/hires/osram-dam-25175617/AS5600_DS000365_5-00.pdf

There are a few good drivers for this guy, but I ran into trouble configuring the Analog output as it is off by default. Not a problem if you only use I2C, but if you dont want to use I2C for your application then this will help you.

Note, I still use I2C to configure it.

## I2C
RobTillart wrote a good library which is used here:

https://github.com/RobTillaart/AS5600

You can put in LIB_deps in platformio.ini

`lib_deps = 
    robtillaart/AS5600@^0.3.4`

This library can be used to read the sensor data and also perform all sorts of configuration and debugging functions.

## Setup
Connect your board, I use an ESP32Devkit01 but I can't see any reason why any other Arduino family board wouldn't work too.
The SDA and SCL pins may vary.

| Pin ESP32 | Pin AS5600 | Notes                               |
| :-------: | :--------: | :---------------------------------- |
|    D4     |    DIR     |                                     |
|    D14    |    OUT     | Analog output                       |
|    D21    |    SDA     |                                     |
|    D22    |    SCL     |                                     |
|    3V3    |    VCC     | AS5600 supports 3V3 or 5V operation |
|    GND    |    GND     |                                     |

## Configuration

The easiest way to configure the chip is to use the I2C bus.
Analog out is disabled by default and will just output a constant 3.3V.

I've made a simple serial interface to help with this. Look in the code for everything it can do, here are the highlights:

Start the serial terminal and you can just enter the following commands:

|  Command  |                                      Action                                       |
| :-------: | :-------------------------------------------------------------------------------: |
|   STAT    |                         Prints a whole lot of debug data                          |
|  ANALOG   | Puts the board in variable voltage output mode, not this must first be configured |
|  PWM920   | Puts the board into PW output mode at 920Hz (lower frequencies are also available |
|  SETMPOS  |              Sets the output range to maximum (ie full 360 degrees)               |
| BURNANGLE |           Burns the output range permanently to the board configuration           |


### Configure Output Mode on OUT pin
The out pin is deactivated by default and just sits at VCC. 
To activate it you need to: 

Set the output range:
`SETMPOS`

Then burn it:
`BURNANGLE`

At this point you should see a voltage proportional to angle on the OUT pin.

If you want PWM then:
`PWM920`



