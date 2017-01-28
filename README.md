# Serial PWM Driver with optional OLED display

##### Version: 1.2

### Number of output channels:
6 (8bit pwm outputs)

    Note: Can handle 12bit inputs too.
    For this function define macro: USE_12BIT_INPUT_VALUES

### Device:
Arduino Nano

### Required input line over serial communication:

| Input line        | channel_num   | value  |
| ----------------- | ------------- | ------ |
| channel_num value | integer 0..5  | 0..255 |


### Optional feautre:
    Can handle simple oled display over I2C or SoftSPI protocols for show current values.
    For this feature define one of following macros: USE_OLED_DISPLAY_I2C or USE_OLED_DISPLAY_SPI
    Used library for handling OLED display:
    [Arduino-SSD1306Ascii](https://github.com/bbkbarbar/Arduino-SSD1306Ascii)

    Testing v1.0 with display:
![Testing @ v1.0](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/blob/master/Documents/Testing_v1_0.png "Testing @ v1.0")


### Further options (not implemented yet):
Handle more than 6 channels (what is the physical limitation by number of arduino's pwm channels)
Plan: for further channels use a second serial bus (softSerial)
for send command for an other instance of Serial PWM driver


#### Created by:
Andras Boor
2017.01.
