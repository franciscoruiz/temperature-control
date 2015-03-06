#! /usr/bin/env python
from glob import glob
import sys

from serial import Serial
from serial import SerialException

from temperature_control import Arduino


def run_terminal():
  arduino = Arduino.autoconnect()
  while True:
    command = sys.stdin.readline().strip()
    response = arduino.send_command(command)
    print response


if __name__ == '__main__':
  try:
    run_terminal()
  except KeyboardInterrupt:
    exit('\n')
  except SerialException:
    exit('Device disconnected.')
