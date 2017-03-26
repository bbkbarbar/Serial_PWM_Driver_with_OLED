/*
 * Serial PWM driver
 * version 1.5
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
 * Optional feature:
 *  -  Display:
 *     Can handle simple oled display for show current values over I2C or SoftSPI
 *     For this feature define one of following macros: 
 *           USE_OLED_DISPLAY_I2C or 
 *           USE_OLED_DISPLAY_SPI
 *     Note: In case of using SPI display, need to define proper values for the following macros:
 *           OLED_DC, OLED_CS, OLED_D0, OLED_D1, OLED_RST
 *           In case of using I2C display, need to define proper value 
 *           for "I2C_ADDRESS_OF_DISPLAY" macro.
 * 
 *  -  Further channels:
 *     Can handle more channel than 6 (what is the physical limitation by number of arduino's pwm channels)
 *     If this feature is in use, then it forwards the received commands for a "slave instance" when 
 *     the channel id (of the received command) is over the range of "own channels".
 *     A second, SoftwareSerial bus is used for forwarding of command to the slace instance of Serial PWM driver
 *     For this feature define the following macro: 
 *           HANDLE_FURTHER_CHANNELS
 * 
 * 
 * Created by: Andras Boor
 * 2017.01.
 */
#define VERSION  "v1.5"


/*
 *  Macro definitions for optional features
 */
#define USE_OLED_DISPLAY_I2C

#define USE_12BIT_INPUT_VALUES

#define HANDLE_FURTHER_CHANNELS

#include <EEPROM.h>


//===============================================================================================


/*
 *  Oprional feature: Handle further channels (pass command for an other instance)
 */
#ifdef HANDLE_FURTHER_CHANNELS

  #include <SoftwareSerial.h>

  // Define witch pins will be used for softSerial output bus for pass commands of further channels (channelId >= 6)
  #define SOFTSERIAL_TX              2
  #define SOFTSERIAL_RX              4
  
  SoftwareSerial serialOutput(SOFTSERIAL_RX, SOFTSERIAL_TX); // RX, TX

  /*
   *  Serial input bus for receive commands
   */
  #define SERIAL_OUTPUT_BAUNDRATE 9600

#endif



/*
 *  Serial input bus for receive commands
 */
#define SERIAL_INPUT_BAUNDRATE    9600



/*
 *  Oprional feature: Handle OLED display over I2C or SoftSPI protocols
 */

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

  // pin definitions for using SPI oled display over softSpi
  #define OLED_D0                   18    // A4
  #define OLED_D1                   17    // A3
  #define OLED_RST                  16    // A2
  #define OLED_DC                   15    // A1
  #define OLED_CS                   14    // A0

  SSD1306AsciiSoftSpi oled;
#endif


/*
 *  Definitions for basic functionality
 */

#define outout_channel_of_red        1
#define outout_channel_of_green      0
#define outout_channel_of_blue       2

#define PWM_OUTPUT_CH0               3
#define PWM_OUTPUT_CH1               5
#define PWM_OUTPUT_CH2               6
#define PWM_OUTPUT_CH3               9
#define PWM_OUTPUT_CH4              10
#define PWM_OUTPUT_CH5              11


#define DEFAULT_OUTPUT_VALUE         0  // can set any value in range [0..255] depending on planned useage

#define PWM_CHANNEL_COUNT            6  // based on the physical limitations of Arduino Nano
#define CHANNEL_UNDEFINED           -1
#define PWM_MAX                    255  // max value of 8bit pwm output
#define DISPLAY_STEP                25  // 1/10 * PWM_MAX
#define QUOTIENT_BETWEEN_8_AND_12_BIT_RESOLUTION         16
#define HALF_OF_QUOTIENT_BETWEEN_8_AND_12_BIT_RESOLUTION  8
#define UPDATE_CHANNEL_INDEPENDENTLY -1

const char pwmChannelMap[PWM_CHANNEL_COUNT] = {
  PWM_OUTPUT_CH0,
  PWM_OUTPUT_CH1,
  PWM_OUTPUT_CH2,
  PWM_OUTPUT_CH3,
  PWM_OUTPUT_CH4,
  PWM_OUTPUT_CH5
};

unsigned char outputs[PWM_CHANNEL_COUNT];     // Array for storing values of pwm channels

String inputBuffer = "";                      // Variable for storing received data


/*
 *  Outputs can be linked to an other channel.
 *  It means the linked new value of a linked channel will be setted on output
 *  only when the other channel (where it is linked) gets a new value.
 *  -1 means that channel is not linked anywhere.
 */
char outputBindings[PWM_CHANNEL_COUNT] = {
	UPDATE_CHANNEL_INDEPENDENTLY,
	UPDATE_CHANNEL_INDEPENDENTLY,
	UPDATE_CHANNEL_INDEPENDENTLY,
	UPDATE_CHANNEL_INDEPENDENTLY,
	UPDATE_CHANNEL_INDEPENDENTLY,
	UPDATE_CHANNEL_INDEPENDENTLY
};

void linkChannel(unsigned char what, unsigned char where){
	if( (what >= PWM_CHANNEL_COUNT) || (where >= PWM_CHANNEL_COUNT) || (what == where)){
		// We can link only the "own" channels together.
		// And we can NOT link a channel for himself.
		return;
	}
	outputBindings[what] = where;
}

//=================================================================================

void setup() {

  // sets the pin for PWM outputs
  pinMode(PWM_OUTPUT_CH0, OUTPUT);   
  pinMode(PWM_OUTPUT_CH1, OUTPUT);   
  pinMode(PWM_OUTPUT_CH2, OUTPUT);   
  pinMode(PWM_OUTPUT_CH3, OUTPUT);   
  pinMode(PWM_OUTPUT_CH4, OUTPUT);   
  pinMode(PWM_OUTPUT_CH5, OUTPUT);   
  
  // Set RGB channels together
  // Set output of ch0 and ch1 only when ch3 is updated.
  linkChannel(0, 2);
  linkChannel(1, 2);


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


  Serial.begin(SERIAL_INPUT_BAUNDRATE);   //Sets the baud for serial data transmission      
  delay(300);
  #ifdef HANDLE_FURTHER_CHANNELS  
    serialOutput.begin(SERIAL_OUTPUT_BAUNDRATE);
    delay(100);
  #endif

  unsigned char wasValueStored = 0;
  for(char i=0; i<PWM_CHANNEL_COUNT; i++){
    //outputs[i] = DEFAULT_OUTPUT_VALUE;
    unsigned char value = EEPROM.read(i);
    EEPROM.write(i, 0);
    outputs[i] = value;
    if(value != 0){
      wasValueStored = 1;
    }
    analogWrite(pwmChannelMap[i], outputs[i]);
  }

  #if defined(USE_OLED_DISPLAY_I2C) || defined(USE_OLED_DISPLAY_SPI)
    // Show start message on display
    oled.set2X();
    //oled.println("Serial\nPWM Driver\n" + String(VERSION));
    oled.println("Serial\nPWM Driver");
    oled.set1X();
    oled.println(String(VERSION) + "\n");

    #ifdef USE_12BIT_INPUT_VALUES
      oled.println("Input mode: 12 bit");
    #else
      oled.println("Input mode: 8 bit");
    #endif
    #ifdef HANDLE_FURTHER_CHANNELS  
      oled.println("Serial out enabled");
    #endif
     
    /*
    int i=0;
    String bindings = "B:";
    for(i=0; i<PWM_CHANNEL_COUNT; i++){
    	if(outputBindings[i] != UPDATE_CHANNEL_INDEPENDENTLY){
	        bindings += (" " + String(i) + "->" + String(outputBindings[i]) + ",");
        }
    }
    oled.println(bindings);

    /**/

  if(wasValueStored){
    showOutputs();
  }

  #endif

}

//=================================================================================

/*
 * Show the current values of own pwm channels on oled display. 
 *
 * Example output:
 *     "ch0:   7 #          "
 *     "ch1: 255 ###########"
 *     "ch2: 127 ######     "
 *     "ch3:   0            "
 *     "ch4:  40 ##         "
 *     "ch5:   0            "
 *
 * Note: this function will be compiled only if 
 *       USE_OLED_DISPLAY_I2C macro or USE_OLED_DISPLAY_SPI macro is used.
 *
 */
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


/*
 * Calculate the appropriate output value 
 * In case of "12bit mode", it calculatates the appropriate value for 8bit wide outputs.
 */
int calculateOutputValue(int input){
  int output;

  #ifdef USE_12BIT_INPUT_VALUES
    output = ((input + HALF_OF_QUOTIENT_BETWEEN_8_AND_12_BIT_RESOLUTION) / QUOTIENT_BETWEEN_8_AND_12_BIT_RESOLUTION);
  #else
    output = input;
  #endif

  // Keep output value in valide range [0..PWM_MAX]
  if(output > PWM_MAX) {
    output = PWM_MAX;
  }else
  if(output < 0) {
    output = 0;
  }

  return output;
}


/*
 * Create line to forward command that wants to set channel for a channel what is out of our range.
 * Calculates new channel id for forwarded command.
 *
 * Example input:
 *     inputChannelId:   8
 *     value:          255
 * Example output:
 *     "2 255"
 *
 * Overall channel 8 means the 2nd channel what is over our range,
 * so the 2nd channel of slave device is the 8th channel in overall.
 *
 * Note: this function will be compiled only if HANDLE_FURTHER_CHANNELS macro is used.
 *
 */
#ifdef HANDLE_FURTHER_CHANNELS
String buildCommandOutput(int inputChannelId, unsigned short value){
  unsigned short shiftedChannelId = inputChannelId - PWM_CHANNEL_COUNT;
  return (String(shiftedChannelId) + " " + String(value));
}
#endif


/*
 * Process input command (what received over serial port)
 * Sample inputs:
 *     - in  8bit mode: "2 255" -> 100% duty cycle of pwm output on channel 2
 *     - in 12bit mode: "0 2047" -> 50% duty cycle of pwm output on channel 0
 * This function does:
 *     - Gets the appropriate output value (in case of "12bit mode", it calculatates the appropriate value for 8bit wide outputs)
 *     - If channelId is within range of "own channels" (0..5) than it sets pwm outputs on specified channel.
 *     - If "HANDLE_FURTHER_CHANNELS" function enabled and the given channelId is out of "own channels" range 
 *       than it forwards the received command with decreased channel id for the "slave" instance.
 *     - If "using oled display" function is enabled then the current values of own channels will be shown on the display
 * 
 */
void processLine(String line){

  //int channel = ((String)(line.charAt(0))).toInt();
  int channel = ((String)(line.substring(0,line.indexOf(' ')))).toInt();
  if((channel < 0) 
      #ifndef HANDLE_FURTHER_CHANNELS
        || (channel >= PWM_CHANNEL_COUNT)
      #endif
  ){
    channel = CHANNEL_UNDEFINED;
  }

  if(channel > CHANNEL_UNDEFINED){
    int value = calculateOutputValue(((String)(line.substring(2))).toInt());
  
    if(channel < PWM_CHANNEL_COUNT){
      outputs[channel] = value;

      // Store current values
      EEPROM.write(channel, value); 
      //if(EEPROM.read(ADDR_COLOR_IS_STORED) == COLOR_IS_STORED){}

      if(outputBindings[channel] == UPDATE_CHANNEL_INDEPENDENTLY){
      
        // Set own pwm output of according to processed channel number and value..
        int i=0;
        for(; i<PWM_CHANNEL_COUNT; i++){
        	if(outputBindings[i] == channel){
        		// channel[i] has been binded to currently updated channel
        		analogWrite(pwmChannelMap[i], outputs[i]);
        	}
        }
        analogWrite(pwmChannelMap[channel], value);

        #if defined(USE_OLED_DISPLAY_I2C) || defined(USE_OLED_DISPLAY_SPI)
          showOutputs();
        #endif
      
      }else{
      	// Output channel has been binded to an other..

      	// Wanted output value is stored in outputs[] so..
      	// Do nothing now..
      }
    }
    #ifdef HANDLE_FURTHER_CHANNELS
      else{ 
        // Forward commands of futher channels for the other device over softSerial bus
        serialOutput.print( buildCommandOutput(channel, value) + "\n" );
      }
    #endif

  }

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
