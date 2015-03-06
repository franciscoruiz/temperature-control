#! /usr/bin/env python
from glob import glob
import sys

from serial import Serial
from serial import SerialException


def connect():
  device_matches = glob('/dev/ttyACM*')
  if not device_matches:
    exit('Cannot find device. Is the Arduino connected?')

  device = device_matches[0]
  baudrate = 9600

  print 'Connecting to device {} ({})'.format(device, baudrate)
  port = Serial(device, baudrate)
  return port


def send_command(port, cmd):
  port.write(cmd)
  response = port.readline()
  return response


def run_terminal():
  port = connect()
  while True:
    command = sys.stdin.readline()
    response = send_command(port, command)
    print response


if __name__ == '__main__':
  try:
    run_terminal()
  except KeyboardInterrupt:
    exit('\n')
  except SerialException:
    exit('Device disconnected.')
