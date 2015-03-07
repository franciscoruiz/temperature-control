#! /usr/bin/env python
import sys

from serial import SerialException

from thermostat.interface import Arduino


def run_terminal():
  arduino = Arduino.autoconnect()
  while True:
    command = sys.stdin.readline().strip()
    response = arduino.send_command(command)
    print response


if __name__ == "__main__":
  try:
    run_terminal()
  except KeyboardInterrupt:
    exit("\n")
  except SerialException:
    exit("Device disconnected.")
