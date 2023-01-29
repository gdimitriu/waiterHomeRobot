# waiterHomeRobot
Waiter robot for home like omnibot2000

## Building phase

### Moving Engine Processor

#### POWER AN CONNECTION OF SENSORS

Connect all VCC and GND from Encoders, IR sensors, PCF8574 and  PCA9685.

Power them using LM78005 and see if all led are lighted and change the distance using semireglables from IR Sensors to desire collision distance.

#### I2C BUS

Connect the I2C Bus.

Upload utils/detectI2Cdevices on arduino nano and should see something like:

00:24:34.569 -> Scanning...

00:24:34.569 -> I2C device found at address 0x21 !

00:24:34.602 -> I2C device found at address 0x40 !

00:24:34.636 -> I2C device found at address 0x70 !

00:24:34.671 -> done

Put the values on moving_engines_processor/configuration:

PCF8574_ADDRESS is the address of PCF8574 ussually 0x20 or something else deppending on 3button switch.

PCA9685_ADDRESS is the address of PCA9685 default is 0x40.

### Second stage

#### LCD TEST

Make connection on LCD.

Uncomment the define TEST_LCD from 2stage_mega/configuration.h

Upload the 2stage_mega to arduino mega 2560.

The LCD should show the information:

- at statup green box should show route 01 and 3 lines.

- in green box sould be route and incresing value and 7 lines which are not changing.

#### SD AND LCD TESTS

Make connection to SD card reader.

Put on SD the data from test_data/tests into tests directory so it should be on SD: tests/sd_lcd.txt.

Put the SD into card reader.

Disable previous LCD test comment the define TEST_LCD from 2stage_mega/configuration.h

Enable the test uncomment the define TEST_SD_LCD from 2stage_mega/configuration.h

Upload the 2stage_mega to arduino mega 2560.

The LCD should show the information:

in green box sould be test/sd_lcd.txt and following lines:

serving first dish get plates

serving main course

get plates

serving desert

get plates

