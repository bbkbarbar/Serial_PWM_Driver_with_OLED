from __future__ import division
import time
import argparse
import os
import datetime

# Constants
output_channel_of_green = 2 	# 0
output_channel_of_blue  = 0
output_channel_of_red   = 1

output_channel_of_red2   = 3
output_channel_of_green2 = 4
output_channel_of_blue2  = 5

# Uncomment to enable debug output.
#import logging
#logging.basicConfig(level=logging.DEBUG)

def sendOutputCommandOnSerial(command):
	print(command + "\n");

# Parse arguments
parser = argparse.ArgumentParser('RGB test')
parser.add_argument("-r","--red", type=int, help="red", default = -1)
parser.add_argument("-g","--green", type=int, help="green", default = -1)
parser.add_argument("-b","--blue", type=int, help="blue", default = -1)

parser.add_argument("-x", "--hex", type=str, help="RGB color in hex string", default = "  ")
parser.add_argument("-c", "--complete", type=str, help="all color component in one", default = "  ")

parser.add_argument("-ch3","--ch3", type=int, help="channel_3", default = 0)
parser.add_argument("-ch4","--ch4", type=int, help="channel_4", default = 0)
parser.add_argument("-ch5","--ch5", type=int, help="channel_5", default = 0)

args = parser.parse_args()

allComponent = args.complete

# Store values for each color-channel
red1   = args.red
green1 = args.green
blue1  = args.blue

red2 = args.ch3
green2 = args.ch4
blue2 = args.ch5


# Set PWM outputs
direction = 1
while True:

	if direction:

		for i in xrange(1,2047):
			r1 = int((red1*i/4096) * i)
			g1 = int((green1*1/4096) * i)
			b1 = int((blue1*1/4096) * i)
			r2 = (red2 * (2048-i)/4096)
			g2 = (green2 * (2048-i)/4096)
			b2 = (blue2 * (2048-i)/4096)
			sendOutputCommandOnSerial(str(output_channel_of_blue)  + " " + str(b1));
			sendOutputCommandOnSerial(str(output_channel_of_red)   + " " + str(r1));
			sendOutputCommandOnSerial(str(output_channel_of_green) + " " + str(g1));
			#sendOutputCommandOnSerial("3 " + str(r2));
			#sendOutputCommandOnSerial("4 " + str(g2));
			#sendOutputCommandOnSerial("5 " + str(b2));

		for i in xrange(2047,16):
			r1 = red1 * i
			g1 = green1 * i
			b1 = blue1 * i
			r2 = red2 * (2048-i)
			g2 = green2 * (2048-i)
			b2 = blue2 * (2048-i)
			sendOutputCommandOnSerial(str(output_channel_of_blue)  + " " + str(b1));
			sendOutputCommandOnSerial(str(output_channel_of_red)   + " " + str(r1));
			sendOutputCommandOnSerial(str(output_channel_of_green) + " " + str(g1));
			sendOutputCommandOnSerial("3 " + str(r2));
			sendOutputCommandOnSerial("4 " + str(g2));
			sendOutputCommandOnSerial("5 " + str(b2));


# Write log
#lasttime = str(datetime.datetime.time(datetime.datetime.now()))[:8]
#write_to_file(lasttime + "\tR:" + str(red) + "\tG: " + str(green) + "\tB: " + str(blue) )
    