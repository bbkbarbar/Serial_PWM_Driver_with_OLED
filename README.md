# Serial PWM Driver with optional OLED display

##### Firmware version: 1.4
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
  - [Serial connection (with a Raspberry Pi Zero)](#serial_communitation_with_pi)
  - [Breadboard testing with all parts on output side](#Breadboard_testing_with_outputs)
- [PCB design](#PCB_design)
  - [Part list](#part_list)
  - [PCB prototype](#PCB_prototype)
  - [First prototype dry test](#prototype_dry_test)
- [Measurements](#Measurements)
  - [Power consumption](#power_consumption)
  - [Memory and program storage utilization on ATmega328](#memory_useage)
- [ToDo list](#todo)
- [Changelog](#changelog)



## <a name="Requirements"> Requirements </a>

This device can control individual PWM outputs as "channels".
It get commands from any other device over serial communication. (e.g.: from a Raspberry Pi or a Bluetooth Serial module)
It need to be able to control any DC device at least with 5A power consumption per channel.
Need to elecrically separete output channels from MCU circuit and from connected serial device.
Output channels 0, 1, 2 (planned to use for RGB) need to be updated in same time
even if any of those channels has no new value (e.g. new commands contains only the red and green values)


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

<a name="testing_protoryping">

## Testing, prototyping

</a>

<a name="serial_communitation_with_pi">

##### Serial connection with a Raspberry Pi Zero

</a>

    Serial PWM Driver board get commands over strandard serial communication.
    In my case the other device (what sends the commands) will be a
    [RaspberryPi Zero](https://www.raspberrypi.org/blog/raspberry-pi-zero/)
    RaspberryPi using serial communication with signal level of 3,3V
    In direction of Pi -> Arduino it works fine, because arduino can read Pi's
    3,3V "high" signal as a "high" signal, since it is significantly higher
    voltage than 2,5V, but:
    In direction of Arduino -> Pi we have to apply a voltage divider to make
    arduino's 5V signal level lower to protect the RaspberryPi.
    For this we can use simple resistors with values of 220 and 470 Ohm,
    to create a 3,3V  signal from Arduino's TX signal.

    Note: According to the measured power consumtion of
    Serial PWM Driver board on MCU side, it can be powered directly
    from VCC of RaspberryPi.
    If it needed because of any reason, Serial PWM Driver board can powered
      - from external VCC (over it's VCC connector)
      - from external raw input (6V-15V over RAW connector)
    Important:
      If Serial PWM Driver board is powered from any external source
      (what is not the same as the power source of connected serial device),
      then the VCC (5V) connection of "Serial In" MUST be interrupted
      (via the jumper on the board).

[MCU side power consumtion can be found below](#power_consumption)

Sample connection between Serial PWM Driver and RaspberryPi:

![Sample connection between Serial PWM Driver and RaspberryPi](https://github.com/bbkbarbar/Serial_PWM_Driver_with_OLED/raw/master/Documents/Connected_to_RaspberryPiZero.png "Sample connection between Serial PWM Driver and RaspberryPi")


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

### <a name="PCB_design"> PCB design </a>
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

| Condition                   | Consumption on VCC   |
| :-------------------------: | :------------------: |
| 0% output on all channels   | 25,6 mA              |
| 100% output on 1 channel    | 29,0 mA              |
| 100% output on 3 channels   | 36,2 mA              |
| 100% output on all channels | 46,8 mA (calculated) |

##### <a name="memory_useage">  Memory and program storage utilization on ATmega328 </a> (Arduino Nano v1.3):

| Option(s) used                              | Program storage  | Dynamic memory  | Sw version |
| :-----------------------------------------: | :--------------: | :-------------: | :--------: |
| With I2C display and using further channels | 9018 bytes (29%) | 429 bytes (20%) | v1.4       |
| Without any optional feature                | 8876 bytes (28%) | 425 bytes (20%) | v1.2       |



### <a name="todo"> ToDo </a>
 - [x] Modify pinout config of softSpi display according to the pin order on display's pcb.
 - [x] Modify PCB design: Leave enaugh space between arduino and optional I2C display (when display connected directly on the board).
 - [X] Modify PCB design: Add pin-header to SoftSPI connection of optional display (it should be 2-sided (rotateable) if it's possible)
 - [X] Add termial connector for using optional 5V power supply for arduino (e.g. get VCC power directly from raspberry)
 - [X] Measure the power consumption before start using with direct 5V power supply (31mA)
 - [X] Check appropirate layout (enaugh space for connect additional boards directly)
 - [X] Create RTU PCB version
 - [X] Build first usable prototype
 - [X] Testing with real output devices
 - [X] Measure power consumption with real output devices
 - [X] Implement requirement to update RGB channels together
 - [ ] Update PCB design with missing labels (hw version, serial output)


### <a name="changelog"> Changelog </a>
    v1.4
      Output channels can be linked together, to be able to update
      RGB channels together in same time
    v1.3
      Add optional feautre for handling more channels with fowarding to an other instance
    v1.2
      Add optional feautre for using I2C or SPI display
    v1.1
      Add optional feautre for handling 8 and 12 bit wide input values
      (output works with 8 bit resolution in all case.)

#### Created by:
    Andras Boor
    2017.01
