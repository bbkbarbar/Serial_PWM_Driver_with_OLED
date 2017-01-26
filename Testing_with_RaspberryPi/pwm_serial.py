from __future__ import division
import time
import argparse
import os
import datetime
import serial

# Constants
outout_channel_of_red   = 1     # 1
outout_channel_of_green = 0 	# 0
outout_channel_of_blue  = 2

def write_to_file(data):
	homePath = os.environ["HOME"]
	with open(homePath + "/logs/rgb_output.log", "a") as myfile:
		myfile.write(data + "\r\n")
		myfile.close()

# Uncomment to enable debug output.
#import logging
#logging.basicConfig(level=logging.DEBUG)

# Initialise the serial output.
ser = serial.Serial(
   port='/dev/ttyAMA0',
   baudrate = 9600,
   parity=serial.PARITY_NONE,
   stopbits=serial.STOPBITS_ONE,
   bytesize=serial.EIGHTBITS,
   timeout=1
)

def sendOutputCommandOnSerial(command):
	ser.write(command + "\n");

# Parse arguments
parser = argparse.ArgumentParser('RGB test')
parser.add_argument("-r","--red", type=int, help="red", default = 0)
parser.add_argument("-g","--green", type=int, help="green", default = 0)
parser.add_argument("-b","--blue", type=int, help="blue", default = 0)

parser.add_argument("-x", "--hex", type=str, help="RGB color in hex string", default = "  ")
parser.add_argument("-c", "--complete", type=str, help="all color component in one", default = "  ")

parser.add_argument("-ch3","--ch3", type=int, help="channel_3", default = -1)
parser.add_argument("-ch4","--ch4", type=int, help="channel_4", default = -1)
parser.add_argument("-ch5","--ch5", type=int, help="channel_5", default = -1)

args = parser.parse_args()

allComponent = args.complete

# Store values for each color-channel
if allComponent[0] == "a":
	red   = int(allComponent[1:4])
	print "red:   " + str(red)
	green = int(allComponent[5:8])
	print "green: " + str(green)
	blue  = int(allComponent[8:10])
	print "blue:  " + str(blue)
else:
	red   = args.red
	green = args.green
	blue  = args.blue

	ch3_val = args.ch3
	ch4_val = args.ch4
	ch5_val = args.ch5


# Set PWM outputs
sendOutputCommandOnSerial(str(outout_channel_of_green) + " " + str(green));
sendOutputCommandOnSerial(str(outout_channel_of_red)   + " " + str(red));
sendOutputCommandOnSerial(str(outout_channel_of_blue)  + " " + str(blue));

if ch3_val >= 0:
	sendOutputCommandOnSerial("3 " + str(ch3_val));
if ch4_val >= 0:
	sendOutputCommandOnSerial("4 " + str(ch4_val));
if ch5_val >= 0:
	sendOutputCommandOnSerial("5 " + str(ch5_val));
#sendOutputCommandOnSerial("3 " + str(ch3_val));
#sendOutputCommandOnSerial("4 " + str(ch4_val));
#sendOutputCommandOnSerial("5 " + str(ch5_val));



# Write log
#lasttime = str(datetime.datetime.time(datetime.datetime.now()))[:8]
#write_to_file(lasttime + "\tR:" + str(red) + "\tG: " + str(green) + "\tB: " + str(blue) )
    