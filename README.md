# Serial PWM Driver with optional OLED display

##### Firmware version: 1.31
##### Hardware version: 1.45

### Device:
Arduino Nano

### Number of output channels:
6 channels with 8 bit PWM outputs - based on physical limitation of arduino board.

    Note: Can handle 12bit inputs too.
    For this function define macro: USE_12BIT_INPUT_VALUES

### Optional feautres:

##### Display can be used (required library: [Arduino-SSD1306 Ascii](https://github.com/bbkbarbar/Arduino-SSD1306Ascii) ):
    Can handle simple oled display over I2C or SoftSPI protocols to show current values.
    For this feature define one of the following macros: USE_OLED_DISPLAY_I2C or USE_OLED_DISPLAY_SPI

##### Handling further channels:
    Can handle more channel than 6 (what is the physical limitation
    by number of pwm channels on Arduino Nano board)
    If this feature is in use, then it forwards the received commands for a "slave instance"
    when the channel id (of the received command) is over the range of "own channels".
    A second, SoftwareSerial bus is used for forwarding of command to the slace instance of Serial PWM driver


### Required input line over serial communication:
Input line: *channel_num value*

| Input mode | Using "further channels" option | channel_num       | value   |
| :--------: | :-----------------------------: | :---------------- | :-----: |
| 8 bit      | No                              | (int) 0..5        | 0..255  |
| 8 bit      | Yes                             | (int) 0..*ch_max* | 0..255  |
| 12 bit     | No                              | (int) 0..5        | 0..4095 |
| 12 bit     | Yes                             | (int) 0..*ch_max* | 0..4095 |
*ch_max*: depens on the count of devices (= 6 * device count)


### Testing v1.0 and v1.2 with display:
v1.0 with I2C display
![Testing @ v1.0 with I2C display](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_v1_0.png "v1.0 with I2C display")
v1.2 with I2C display and sample output RGB leds
![v1.2 with I2C display and sample output RGB leds](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Breadboard_Testing_v1.2.png "v1.2 with I2C display and sample output RGB leds")

### Testing v1.3 with 2 instances with I2C and SPI displays:

Two instances (left one is the "master device" with blue I2C display and "FURTHER_CHANNELS" feature enabled, right one with white SPI display)
![Two instances (left one is the "master device" with blue I2C display and "FURTHER_CHANNELS" feature enabled, right one with white SPI display)](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_-_v1.3_with_2_instances_1.png)

Send pwm commands from raspberryPi using the [attached python script](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/blob/master/Testing_with_RaspberryPi/pwm_serial_12ch.py):
![Send pwm commands from raspberryPi with attached python script](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_-_Send_commands_from_Pi.png "Send pwm commands from raspberryPi with attached python script")

Two instances after command has been received
![Two instances after command has been received](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_-_v1.3_with_2_instances_2.png "Two instances after command has been received")

### Testing sw v1.3 and hw v1.44 with all parts on output side in breadboard
![Testing with 0% output](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/raw/master/Documents/Overall_testing/Control_12V_LED_strip_Powered_from_PI_5V_OFF.png)

![Testing with 100% output](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/raw/master/Documents/Overall_testing/Control_12V_LED_strip_Powered_from_PI_5V_ON.png)

### Memory and program storage utilization on ATmega328 (Arduino Nano v1.3):

| Option(s) used                              | Program storage  | Dynamic memory  |
| :-----------------------------------------: | :--------------: | :-------------: |
| With I2C display and using further channels | 4312 bytes (14%) | 212 bytes (10%) |
| Without any optional feature                | 8876 bytes (28%) | 425 bytes (20%) |



![PCB design v1.45](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/PCB_design_1.45.png)

### Eagle schamatic available as zip:
[Serial_PWM_Driver_eagle_design](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/blob/master/Schematic_designs/Eagle_designs/Serial_PWM_driver_-_Eagle_desing.zip)
<br>[PCB design as PDF](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/raw/master/Schematic_designs/Eagle_designs/PCB_design_v1.45_bottom.pdf)


### ToDo:
 - [x] Modify pinout config of softSpi display according to the pin order on display's pcb.
 - [x] Modify PCB design: Leave enaugh space between arduino and optional I2C display (when display connected directly on the board).
 - [X] Modify PCB design: Add pin-header to SoftSPI connection of optional display (it should be 2-sided (rotateable) if it's possible)
 - [X] Add termial connector for using optional 5V power supply for arduino (e.g. get VCC power directly from raspberry)
 - [ ] Measure the power consumption before start using with direct 5V power supply)
 - [ ] Check appropirate layout (enaugh space for connect additional boards directly)

#### Created by:
Andras Boor
2017.01.
