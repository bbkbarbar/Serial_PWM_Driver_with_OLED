# Serial PWM Driver with optional OLED display

### Number of output channels: 
6 (8bit pwm outputs)

### Device: 
Arduino Nano

Required input line over serial communication:

| Input line        | channel_num   | value  |
| ----------------- | ------------- | ------ |
| channel_num value | integer 0..5  | 0..255 |


### Optional feautre:
    Can handle simple I2C oled display for show current values.
    For this feature define macro: USE_OLED_DISPLAY
    Inline-style: 
![Testing @ v1.0](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/blob/master/Documents/Testing_v1_0.png "Testing @ v1.0")


### Further options (not implemented yet):
Handle more channel than 6 (what is the physical limitation by number of arduino's pwm channels)
Plan: for further channels use a second serial line
for send command for an other instance of Serial PWM driver


### Used library (needed only for using OLED display):
[Arduino-SSD1306Ascii](https://github.com/bbkbarbar/Arduino-SSD1306Ascii)


#### Created by: 
Andras Boor
2017.01.
