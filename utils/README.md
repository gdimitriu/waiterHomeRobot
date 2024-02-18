# Utils to calibrate or operate platform

## ultralight_write_read

This use arduino UNO.

This is used to read and write NFC tag that holds the data for path.

## CalibrationEncoder

This use the arduino nano from the moving_engines_processor.

This is used to read the number of click for each encoder.

The wheel should be move manually.

## pcf8574_int_sensors

This use the arduino nano from the moving_engines_processor.

This is used to check if the sensors are functional.

This is the same part as is in the moving_engines_processor.

## detectI2Cdevices

This use the arduino nano from the moving_engines_processor or arduino mega from 2 stage.

This is used to see devices address which are connected to the I2C bus.

## 4engines_calibrations 

This use the arduino nano from moving_engines_processor to check if the platform is moving as desired.


## testSerial

This is used to test Bluetooth with moving engine processor.

This is also used to test Serial communication between stage 2 and moving engine processor.
