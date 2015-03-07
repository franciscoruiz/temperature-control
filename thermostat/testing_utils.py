import random


class MockThermostat(object):

  def __init__(self):
    self._is_active = False
    self._status = False
    self.target_temperature = 20.0

  def set_temperature(self, temperature):
    self.target_temperature = temperature

  def get_temperature(self):
    random_delta = -3 + random.random() * 6  # Random in [-3, +3]
    current_temperature = self.target_temperature + random_delta
    return current_temperature

  def is_active(self):
    return self._is_active

  def switch_on(self):
    self._is_active = True

  def switch_off(self):
    self._is_active = False

  def get_status(self):
    return self.get_temperature() < self.target_temperature
