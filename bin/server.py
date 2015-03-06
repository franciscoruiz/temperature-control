from glob import glob
import struct
import sys

from flask import Flask
from flask import jsonify
from flask import Response
from serial import Serial
from serial import SerialException


app = Flask(__name__)


@app.route("/get-temperature/")
def get_temperature():
  temperature = temperature_control.get_temperature()
  return jsonify(temperature=temperature)


@app.route("/set-temperature/<float:temperature>", methods=["POST"])
def set_temperature(temperature):
  temperature_control.set_temperature()
  return Response(status=204)


class Arduino(object):

  baudrate = 9600

  def __init__(self, device):
    self._port = Serial(device, self.baudrate)
    print 'Connecting to device {} ({})'.format(device, baudrate)

  @classmethod
  def autoconnect(cls):
    device_matches = glob('/dev/ttyACM*')
    if not device_matches:
      exit('Cannot find device. Is the Arduino connected?')

    return cls(device_matches[0])

  def send_command(self, cmd):
    self._port.write(cmd + '\n')
    response = self._port.readline()
    print cmd
    print response
    return response


class TemperatureControl(Arduino):

  def switch_on(self):
    self.send_command("ON")

  def switch_off(self):
    self.send_command("OFF")

  def set_temperature(self, temperature):
    self.send_command("SET {}".format(temperature))

  def get_temperature(self):
    temperature_raw = self.send_command("GET")
    temperature = struct.unpack("f", temperature_raw)
    return temperature


if __name__ == "__main__":
  # Connect to Arduino
  temperature_control = TemperatureControl.autoconnect()

  # Start server. See http://localtunnel.me/
  app.run(host='0.0.0.0', debug=True)
