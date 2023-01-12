# AS5600 sensor setup

## Analog out

Analog out is disabled by default and will just output a constant 3.3V.

To activate it first the max angle needs to be set. The range is 0-4096 corresponding to 0-360 degrees:

`as5600.setMPosition(4096);`

Then the data needs to be burned to the board:

`as5600.burnAngle();`

At this point the board should be outputting a voltage proportional to the angle of the shaft.

