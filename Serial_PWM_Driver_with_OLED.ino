/*
 * Serial PWM driver
 * version 1.2
 * 
 * Device: 
 *     Arduino Nano
 * Number of output channels: 
 *     6 (8bit pwm outputs)
 *
 * Required input line over serial communication:
 * "<channel_num> <value>"
 * Where:
 *     <channel_num> is an integer in range [0..5]
 *     <value> is an integer for pwm value
 *     
 * Input value can be a 8bit pwm value or 12bit pwmvalue.
 * Note: For handle 12bit pwm inputs define the following macro:
 *       USE_12BIT_INPUT_VALUES
 *
 *
 * Optional feautre:
 *     Can handle simple oled display for show current values over I2C or SoftSPI
 *     For this feature define one of following macros: USE_OLED_DISPLAY_I2C or USE_OLED_DISPLAY_SPI
 *     Note: In case of using SPI display, need to define proper values for the following macros:
 *           OLED_DC, OLED_CS, OLED_D0, OLED_D1, OLED_RST
 *           In case of using I2C display, need to define proper value 
 *           for "I2C_ADDRESS_OF_DISPLAY" macro.
 * 
 * 
 * Further options (not implemented yet):
 * Handle more channel than 6 (what is the physical limitation by number of arduino's pwm channels)
 * Plan: for further channels use a second SoftwareSerial bus
 * for send command for an other instance of Serial PWM driver
 * 
 * 
 * Created by: Andras Boor
 * 2017.01.
 */
#define VERSION  "v1.2"


#define USE_OLED_DISPLAY_I2C

#define USE_12BIT_INPUT_VALUES


#if defined(USE_OLED_DISPLAY_I2C) || defined(USE_OLED_DISPLAY_SPI)
  #include "SSD1306Ascii.h"
#endif



#ifdef USE_OLED_DISPLAY_I2C
  #include "SSD1306AsciiAvrI2c.h"
  
  #define I2C_ADDRESS_OF_DISPLAY  0x3C  // 0X3C+SA0 - 0x3C or 0x3D
  
  SSD1306AsciiAvrI2c oled;
#endif



#ifdef USE_OLED_DISPLAY_SPI
  #include "SSD1306AsciiSoftSpi.h"

  // pin definitions for using SPI oled display with softSpi
  #define OLED_DC               14
  #define OLED_CS               15
  #define OLED_D0               16
  #define OLED_D1               17
  #define OLED_RST              18  

  SSD1306AsciiSoftSpi oled;
#endif



#define outout_channel_of_red    1
#define outout_channel_of_green  0
#define outout_channel_of_blue   2

#define PWM_OUTPUT_CH0           3
#define PWM_OUTPUT_CH1           5
#define PWM_OUTPUT_CH2           6
#define PWM_OUTPUT_CH3           9
#define PWM_OUTPUT_CH4          10
#define PWM_OUTPUT_CH5          11


#define DEFAULT_OUTPUT_VALUE     0    // can set any value in range [0..255] depending on planned useage

#define PWM_CHANNEL_COUNT        6    // based on the physical limitations of Arduino Nano
#define CHANNEL_UNDEFINED       -1
#define PWM_MAX                255    // max value of 8bit pwm signal
#define DISPLAY_STEP            25
#define QUOTIENT_BETWEEN_8_AND_12_BIT_RESOLUTION         16
#define HALF_OF_QUOTIENT_BETWEEN_8_AND_12_BIT_RESOLUTION  8

const char pwmChannelMap[PWM_CHANNEL_COUNT] = {
  PWM_OUTPUT_CH0,
  PWM_OUTPUT_CH1,
  PWM_OUTPUT_CH2,
  PWM_OUTPUT_CH3,
  PWM_OUTPUT_CH4,
  PWM_OUTPUT_CH5
};


String inputBuffer = "";                      // Variable for storing received data
unsigned char outputs[PWM_CHANNEL_COUNT];     // Array for storing values of pwm channels


//------------------------------------------------------------------------------

void setup() {

  // sets the pin for PWM outputs
  pinMode(PWM_OUTPUT_CH0, OUTPUT);   
  pinMode(PWM_OUTPUT_CH1, OUTPUT);   
  pinMode(PWM_OUTPUT_CH2, OUTPUT);   
  pinMode(PWM_OUTPUT_CH3, OUTPUT);   
  pinMode(PWM_OUTPUT_CH4, OUTPUT);   
  pinMode(PWM_OUTPUT_CH5, OUTPUT);   
  
  #ifdef USE_OLED_DISPLAY_I2C
    oled.begin(&Adafruit128x64, I2C_ADDRESS_OF_DISPLAY, true);
  #endif
  #ifdef USE_OLED_DISPLAY_SPI
    oled.begin(&Adafruit128x64, OLED_CS, OLED_DC, OLED_D0, OLED_D1, OLED_RST);
  #endif
  #if defined(USE_OLED_DISPLAY_I2C) || defined(USE_OLED_DISPLAY_SPI)
    oled.setFont(Adafruit5x7);  
    oled.clear();  
  #endif

  Serial.begin(9600);   //Sets the baud for serial data transmission      
  delay(300);

  for(char i=0; i<PWM_CHANNEL_COUNT; i++){
    outputs[i] = DEFAULT_OUTPUT_VALUE;
  }

  #if defined(USE_OLED_DISPLAY_I2C) || defined(USE_OLED_DISPLAY_SPI)
    // Show start message on display
    oled.set2X();
    oled.println("Serial\nPWM Driver\n" + String(VERSION));
    oled.set1X();

    #ifdef USE_12BIT_INPUT_VALUES
      oled.println("\nInput mode: 12bit");
    #else
      oled.println("\nInput mode: 8bit");
    #endif

    //oled.println("Listen serial port..");
  #endif
}

//------------------------------------------------------------------------------


#if defined(USE_OLED_DISPLAY_I2C) || defined(USE_OLED_DISPLAY_SPI)
void showOutputs(){
  oled.clear();
  for(int ch=0; ch<PWM_CHANNEL_COUNT; ch++){
    byte barLength = (outputs[ch] / DISPLAY_STEP);
    String bar = " ";
    for(byte i=0; i<barLength; i++){
      bar += "#";
    }
    if((outputs[ch] > 0) && (barLength == 0)){
      bar += "#";
    }
    oled.println("ch" + String(ch) + ": " + (outputs[ch]<100?" ":"") + (outputs[ch]<10?" ":"") + outputs[ch] + bar);
  }
}
#endif


int calculateOutputValue(int input){
  int output;
  #ifdef USE_12BIT_INPUT_VALUES
    output = ((input + HALF_OF_QUOTIENT_BETWEEN_8_AND_12_BIT_RESOLUTION) / QUOTIENT_BETWEEN_8_AND_12_BIT_RESOLUTION);
  #else
    output = input;
  #endif
  return output;
}


void processLine(String line){

  int channel = ((String)(line.charAt(0))).toInt();
  if((channel < 0) || (channel >= PWM_CHANNEL_COUNT)){
    channel = CHANNEL_UNDEFINED;
  }

  if(channel > CHANNEL_UNDEFINED){
    int value = calculateOutputValue(((String)(line.substring(2))).toInt());
    if(value > PWM_MAX) {
      value = PWM_MAX;
    }
  
    if(channel < PWM_CHANNEL_COUNT){
      outputs[channel] = value;
      // Set pwm output of according to processed channel number and value..
      analogWrite(pwmChannelMap[channel], value);
    }
  }

  #if defined(USE_OLED_DISPLAY_I2C) || defined(USE_OLED_DISPLAY_SPI)
    showOutputs();
  #endif
}


void loop(){

  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (Serial.available() > 0) {
      char c = Serial.read();  //gets one byte from serial buffer
      if(c != '\n'){
        inputBuffer += c; //build an inputBuffer string from received charachters
      }else{
        processLine(inputBuffer);
        inputBuffer = "";
      }
    } 
  }

}
