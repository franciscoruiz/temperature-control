#! /usr/bin/env python
from glob import glob
import sys

from serial import Serial
from serial import SerialException


device_matches = glob('/dev/ttyACM*')
if not device_matches:
  exit('Cannot find device. Is the Arduino connected?')

device = device_matches[0]
baudrate = 9600

print 'Connecting to device {} ({})'.format(device, baudrate)
port = Serial(device, baudrate)
port.write(sys.argv[1] + '\n')
port.close()
