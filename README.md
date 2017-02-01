# Serial PWM Driver with optional OLED display

##### Sw Version: 1.3

### Number of output channels:
6 (8bit pwm outputs)

    Note: Can handle 12bit inputs too.
    For this function define macro: USE_12BIT_INPUT_VALUES

### Device:
Arduino Nano

### Required input line over serial communication:

| Input mode | Input line        | channel_num  | value   |
| :--------: | :---------------: | :----------: | :-----: |
| 8 bit      | channel_num value | integer 0..5 | 0..255  |
| 12 bit     | channel_num value | integer 0..5 | 0..4095 |


### TODO:
   - create table here with SW and suitable HW versions

### Optional feautres:
##### Display can be attached:
    Can handle simple oled display over I2C or SoftSPI protocols for show current values.
    For this feature define one of following macros: USE_OLED_DISPLAY_I2C or USE_OLED_DISPLAY_SPI
    Used library for handling OLED display:

###### [Arduino-SSD1306Ascii](https://github.com/bbkbarbar/Arduino-SSD1306Ascii)

##### Handling further channels:
    Can handle more channel than 6 (what is the physical limitation by number of arduino's pwm channels)
    If this feature is in use, then it forwards the received commands for a "slave instance" when the channel id (of the received command) is over the range of "own channels".
    A second, SoftwareSerial bus is used for forwarding of command to the slace instance of Serial PWM driver



    Testing v1.0 with display:
![Testing @ v1.0](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/blob/master/Documents/Testing_v1_0.png "Testing @ v1.0")
![Testing @ v1.2](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Breadboard_Testing_v1.2.png "Testing @ v1.2")

### Further options (not implemented yet):
Handle more than 6 channels (what is the physical limitation by number of arduino's pwm channels)
Plan: for further channels use a second serial bus (softSerial)
for send command for an other instance of Serial PWM driver


### Eagle schamatic available as zip:
[Serial_PWM_Driver_eagle_design](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/blob/master/Schematic_designs/Eagle_designs/Serial_PWM_driver_-_Eagle_desing.zip)


#### Created by:
Andras Boor
2017.01.
