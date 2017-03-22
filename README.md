# Serial PWM Driver with optional OLED display

##### Firmware version: 1.31
##### Hardware version: 1.45

#### Table of content:
- [Requirements](#Requirements)
  - [Used MCU](#Device)
  - [Number of output channels](#output_channels)
- [Optional feautres](#Optional_feautres)
  - [Display can be used](#Display_can_be_used)
  - [Handling further channels](#further_channels)
- [Required input line over serial communication](#Required_input_line)
- [Testing, prototyping](#testing_protoryping)
  - [Breadboard testing with all parts on output side](#Breadboard_testing_with_outputs)
- [PCB design](#PCB_design)
  - [Part list](#part_list)
  - [PCB prototype](#PCB_prototype)
  - [First prototype dry test](#prototype_dry_test)
- [Measurements](#Measurements)
  - [Power consumption](#power_consumption)
  - [Memory and program storage utilization on ATmega328](#memory_useage)
- [ToDo list](#todo)



## <a name="Requirements"> Requirements </a>

This device can control individual PWM outputs as "channels".
It get commands from any other device over serial communication. (e.g.: from a Raspberry Pi or a Bluetooth Serial module)
It need to be able to control any DC device at least with 5A power consumption per channel.
Need to elecrically separete output channels from MCU circuit and from connected serial device.


##### <a name="Device"> Used MCU: </a>
Arduino Nano v3 [(eBay link)](http://www.ebay.com/itm/191773759569?_trksid=p2057872.m2749.l2649&ssPageName=STRK%3AMEBIDX%3AIT)

##### <a name="output_channels"> Number of output channels: </a>
6 channels with 8 bit PWM outputs - based on physical limitation of arduino board.

    Note: Can handle 12bit inputs too.
    For this function define macro:

        USE_12BIT_INPUT_VALUES


### <a name="Optional_feautres"> Optional feautres: </a>

###### <a name="Display_can_be_used"> Display can be used </a> (required library: [Arduino-SSD1306 Ascii](https://github.com/bbkbarbar/Arduino-SSD1306Ascii) ):
    Can handle simple oled display over I2C or SoftSPI protocols to show current values.
    For this feature define one of the following macros:

        USE_OLED_DISPLAY_I2C or
        USE_OLED_DISPLAY_SPI


###### <a name="further_channels"> Handling further channels: </a>
    Can handle more than 6 channels (what is the physical limitation
    by number of pwm channels on Arduino Nano board)
    If this feature is in use, then it forwards the received commands for a "slave instance"
    when the channel id (of the received command) is over the range of "own channels".
    A second, SoftwareSerial bus is used for forwarding of command to the slave instance of Serial PWM driver
    For this feature define the following macro:

        HANDLE_FURTHER_CHANNELS

### <a name="Required_input_line"> Required input line over serial communication: </a>
    Input line: channel_num value

    | Input mode | Using "further channels" option | channel_num | value   |
    | ---------- | ------------------------------- | ----------- | ------- |
    |    8 bit   |              No                 | 0..5        | 0.. 255 |
    |    8 bit   |              Yes                | 0..ch_max   | 0.. 255 |
    |   12 bit   |              No                 | 0..5        | 0..4095 |
    |   12 bit   |              Yes                | 0..ch_max   | 0..4095 |

    ch_max: depens on the count of devices (= 6 * device count)

    Examples: 0 255
              5 4095

## <a name="testing_protoryping"> Testing, prototyping </a>

##### Testing v1.0 and v1.2 with display:
v1.0 with I2C display
![Testing @ v1.0 with I2C display](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_v1_0.png "v1.0 with I2C display")
v1.2 with I2C display and sample output RGB leds
![v1.2 with I2C display and sample output RGB leds](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Breadboard_Testing_v1.2.png "v1.2 with I2C display and sample output RGB leds")

##### Testing v1.3 with 2 instances with I2C and SPI displays:

Two instances (left one is the "master device" with blue I2C display and "FURTHER_CHANNELS" feature enabled, right one with white SPI display)
![Two instances (left one is the "master device" with blue I2C display and "FURTHER_CHANNELS" feature enabled, right one with white SPI display)](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_-_v1.3_with_2_instances_1.png)

Send pwm commands from raspberryPi using the [attached python script](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/blob/master/Testing_with_RaspberryPi/pwm_serial_12ch.py):
![Send pwm commands from raspberryPi with attached python script](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_-_Send_commands_from_Pi.png "Send pwm commands from raspberryPi with attached python script")

Two instances after command has been received
![Two instances after command has been received](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/Testing_-_v1.3_with_2_instances_2.png "Two instances after command has been received")


##### <a name="Breadboard_testing_with_outputs"> Breadboard testing with all parts on output side </a>
    Firmware:          v1.3
    Used layout as Hw: v1.44

![Testing with 0% output](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/raw/master/Documents/Overall_testing/Control_12V_LED_strip_Powered_from_PI_5V_OFF.png)

![Testing with 100% output](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/raw/master/Documents/Overall_testing/Control_12V_LED_strip_Powered_from_PI_5V_ON.png)

## <a name="PCB_design"> PCB design </a>
![PCB design v1.45](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Documents/PCB_design_1.45.png)

##### Eagle schamatic available as zip & pdf:
[Serial_PWM_Driver_eagle_design](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/blob/master/Schematic_designs/Eagle_designs/Serial_PWM_driver_-_Eagle_desing.zip)
<br>[PCB design as PDF](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/raw/master/Schematic_designs/Eagle_designs/PCB_design_v1.45_bottom.pdf)

##### <a name="part_list"> Part list </a> available as [HeStore](https://www.hestore.hu/) basket:
 - [Complete](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Schematic_designs/Part%20lists/hestore_buy_list_v1.45_Complete.xml)
 - [Without arduino](https://raw.githubusercontent.com/bbkbarbar/Serial_PWM_Driver_with_OLED/master/Schematic_designs/Part%20lists/hestore_buy_list_v1.45_without_Arduino.xml)

##### <a name="PCB_prototype"> PCB prototype </a> (hw v1.45)
![PCB prototype 1](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/raw/master/Documents/Board_prototype/pcb_prototype_1.png)

![PCB prototype 2](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/raw/master/Documents/Board_prototype/pcb_prototype_2.png)

##### <a name="prototype_dry_test"> First prototype dry test </a>
 - Firmware v1.3
 - Hw v1.45

![First prototype dry test](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/raw/master/Documents/Overall_testing/Prototype_dry_test.png)


### <a name="Measurements"> Measurements </a>

##### <a name="power_consumption"> Power consumption </a>
On MCU side:

| Condition                   | Consumption on VCC    |
| :-------------------------: | :-------------------: |
| 0% output on all channels   | 31,0 mA               |
| 100% output on 1 channel    | 34,4 mA               |
| 100% output on all channels | 51,4 mA (calculated)  |

##### <a name="memory_useage">  Memory and program storage utilization on ATmega328 </a> (Arduino Nano v1.3):

| Option(s) used                              | Program storage  | Dynamic memory  |
| :-----------------------------------------: | :--------------: | :-------------: |
| With I2C display and using further channels | 4312 bytes (14%) | 212 bytes (10%) |
| Without any optional feature                | 8876 bytes (28%) | 425 bytes (20%) |



### <a name="todo"> ToDo: </a>
 - [x] Modify pinout config of softSpi display according to the pin order on display's pcb.
 - [x] Modify PCB design: Leave enaugh space between arduino and optional I2C display (when display connected directly on the board).
 - [X] Modify PCB design: Add pin-header to SoftSPI connection of optional display (it should be 2-sided (rotateable) if it's possible)
 - [X] Add termial connector for using optional 5V power supply for arduino (e.g. get VCC power directly from raspberry)
 - [X] Measure the power consumption before start using with direct 5V power supply (31mA)
 - [X] Check appropirate layout (enaugh space for connect additional boards directly)
 - [X] Create RTU PCB version
 - [X] Build first usable prototype
 - [ ] Testing with real output devices
 - [ ] Measure power consumption with real output devices
 - [ ] Update PCB design with missing labels (hw version, serial output)

#### Created by:
    Andras Boor
    2017.01

Test line only
