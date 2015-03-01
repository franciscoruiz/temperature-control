#! /usr/bin/env python
from glob import glob
import sys

from serial import Serial
from serial import SerialException


device_matches = glob('/dev/ttyACM*')
if not device_matches:
  exit('Cannot find device. Is the Arduino connected?')

device = device_matches[0]

if len(sys.argv) == 2:
  baudrate = int(sys.argv[1])
else:
  baudrate = 9600


print 'Connecting to device {} ({})'.format(device, baudrate)
port = Serial(device, baudrate)
try:
  while True:
    print port.readline().strip()
except KeyboardInterrupt:
  print
  exit()
except SerialException:
  exit('Device disconnected.')

