from glob import glob
import sys

from serial import Serial


class ThermostatException(Exception):

  pass


class DeviceNotFoundException(ThermostatException):

  pass


class Arduino(object):

  baudrate = 9600

  def __init__(self, device):
    self._port = Serial(device, self.baudrate)
    print "Connecting to device {} ({})".format(device, self.baudrate)

  @classmethod
  def autoconnect(cls):
    device_matches = glob("/dev/ttyACM*")
    if not device_matches:
      raise DeviceNotFoundException()

    return cls(device_matches[0])

  def send_command(self, cmd):
    self._port.write(cmd + "\n")
    response = self._port.readline().strip()
    return response


class Thermostat(Arduino):

  def __init__(self, *args, **kwargs):
    super(Thermostat, self).__init__(*args, **kwargs)

    self.target_temperature = 0.0

  def set_temperature(self, temperature):
    self.target_temperature = temperature
    self.send_command("SET {}".format(temperature))

  def get_temperature(self):
    temperature_raw = self.send_command("GET")
    temperature = float(temperature_raw)
    return temperature

  def is_active(self):
    return bool(int(self.send_command("ACTIVE")))

  def switch_on(self):
    self.send_command("ON")

  def switch_off(self):
    self.send_command("OFF")

  def get_status(self):
    return bool(int(self.send_command("STATUS")))
