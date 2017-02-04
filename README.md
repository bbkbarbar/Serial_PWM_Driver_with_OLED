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


### Optional feautres:

##### Display can be attached (used library: [Arduino-SSD1306Ascii](https://github.com/bbkbarbar/Arduino-SSD1306Ascii) ):
    Can handle simple oled display over I2C or SoftSPI protocols for show current values.
    For this feature define one of the following macros: USE_OLED_DISPLAY_I2C or USE_OLED_DISPLAY_SPI
    Used library for handling OLED display:

##### Handling further channels:
    Can handle more channel than 6 (what is the physical limitation by number of arduino's pwm channels)
    If this feature is in use, then it forwards the received commands for a "slave instance"
    when the channel id (of the received command) is over the range of "own channels".
    A second, SoftwareSerial bus is used for forwarding of command to the slace instance of Serial PWM driver



### Testing v1.0 and v1.2 with display:
v1.0 with I2C display
![Testing @ v1.0 with I2C display](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_v1_0.png "v1.0 with I2C display")
v1.2 with I2C display and sample output RGB leds
![v1.2 with I2C display and sample output RGB leds](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Breadboard_Testing_v1.2.png "v1.2 with I2C display and sample output RGB leds")

### Testing v1.3 with 2 instances with I2C and SPI displays:

Two instances (left one is the "master device" with blue I2C display and "FURTHER_CHANNELS" feature enabled, right one with white SPI display)
![Two instances (left one is the "master device" with blue I2C display and "FURTHER_CHANNELS" feature enabled, right one with white SPI display)](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_-_v1.3_with_2_instances_1.png "Two instances (left one is the "master device" with blue I2C display and "FURTHER_CHANNELS" feature enabled, right one with white SPI display)")

Send pwm commands from raspberryPi using the [attached python script](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/blob/master/Testing_with_RaspberryPi/pwm_serial_12ch.py):
![Send pwm commands from raspberryPi with attached python script](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_-_Send_commands_from_Pi.png "Send pwm commands from raspberryPi with attached python script")

Two instances after command has been received
![Two instances after command has been received](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_-_v1.3_with_2_instances_2.png "Two instances after command has been received")

### Further options (not implemented yet):
Handle more than 6 channels (what is the physical limitation by number of arduino's pwm channels)
Plan: for further channels use a second serial bus (softSerial)
for send command for an other instance of Serial PWM driver

### Memory and program storage utilization on ATmega328 (Arduino Nano v1.3):

| Option(s) used                              | Program storage  | Dynamic memory  |
| :-----------------------------------------: | :--------------: | :-------------: |
| With I2C display and using further channels | 4312 bytes (14%) | 212 bytes (10%) |
| Without any optional feature                | 8876 bytes (28%) | 425 bytes (20%) |


### Eagle schamatic available as zip:
[Serial_PWM_Driver_eagle_design](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/blob/master/Schematic_designs/Eagle_designs/Serial_PWM_driver_-_Eagle_desing.zip)


### ToDo:
 - [ ] Modify PCB design to leave enaugh space between arduino and optional I2C display (when display connected directly on the board).


#### Created by:
Andras Boor
2017.01.
