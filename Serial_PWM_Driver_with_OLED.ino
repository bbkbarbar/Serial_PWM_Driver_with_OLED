/*
 * Serial PWM driver
 * # of output channels: 6 (8bit pwm outputs)
 * 
 * Device: 
 *     Arduino Nano
 *
 * Required input line over serial communication:
 * "<channel_num> <value>"
 * Where:
 *     <channel_num> is an integer in range [0..5]
 *     <value> is an integer for pwm value
 *
 *
 * Optional feautre:
 *     Can handle simple I2C oled display for show current values.
 *     For this feature define macro: USE_OLED_DISPLAY
 * 
 * 
 * Further options (not implemented yet):
 * Handle more channel than 6 (what is the physical limitation by number of arduino's pwm channels)
 * Plan: for further channels use a second serial line
 * for send command for an other instance of Serial PWM driver
 * 
 * 
 * Created by: Andras Boor
 * 2017.01.
 */


#define USE_OLED_DISPLAY

#ifdef USE_OLED_DISPLAY

  #include "SSD1306Ascii.h"
  #include "SSD1306AsciiAvrI2c.h"

  #define I2C_ADDRESS         0x3C  // 0X3C+SA0 - 0x3C or 0x3D
  SSD1306AsciiAvrI2c oled;
  
#endif


#define outout_channel_of_red    0
#define outout_channel_of_green  1
#define outout_channel_of_blue   2

#define PWM_OUTPUT_CH0           3
#define PWM_OUTPUT_CH1           5
#define PWM_OUTPUT_CH2           6
#define PWM_OUTPUT_CH3           9
#define PWM_OUTPUT_CH4          10
#define PWM_OUTPUT_CH5          11


#define DEFAULT_OUTPUT_VALUE     0

#define PWM_CHANNEL_COUNT        6
#define CHANNEL_UNDEFINED       -1
#define PWM_MAX                255
#define DISPLAY_STEP            25

const char pwmChannelMap[PWM_CHANNEL_COUNT] = {
  PWM_OUTPUT_CH0,
  PWM_OUTPUT_CH1,
  PWM_OUTPUT_CH2,
  PWM_OUTPUT_CH3,
  PWM_OUTPUT_CH4,
  PWM_OUTPUT_CH5
};


String inputBuffer = "";                       // Variable for storing received data
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
  
  #ifdef USE_OLED_DISPLAY
    oled.begin(&Adafruit128x64, I2C_ADDRESS, true);
    oled.setFont(Adafruit5x7);  
    oled.clear();  
  #endif

  Serial.begin(9600);   //Sets the baud for serial data transmission      
  delay(300);

  for(char i=0; i<PWM_CHANNEL_COUNT; i++){
    outputs[i] = DEFAULT_OUTPUT_VALUE;
  }

  #ifdef USE_OLED_DISPLAY
    // Show start message on display
    oled.set2X();
    oled.println("Serial\nPWM Driver\nv1.0");
    oled.set1X();
    oled.println("\nListen serial port..");
  #endif
}

//------------------------------------------------------------------------------


#ifdef USE_OLED_DISPLAY
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

void processLine(String line){

  int channel = ((String)(line.charAt(0))).toInt();
  if((channel < 0) || (channel >= PWM_CHANNEL_COUNT)){
    channel = CHANNEL_UNDEFINED;
  }

  if(channel > CHANNEL_UNDEFINED){
    int value = ((String)(line.substring(2))).toInt();
    if(value > PWM_MAX) {
      value = PWM_MAX;
    }
  
    if(channel < PWM_CHANNEL_COUNT){
      outputs[channel] = value;
      // Set pwm output of according to processed channel number and value..
      analogWrite(pwmChannelMap[channel], value);
    }
  }

  #ifdef USE_OLED_DISPLAY
    showOutputs();
  #endif
}


void loop(){

  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill 
    if (Serial.available() >0) {
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
