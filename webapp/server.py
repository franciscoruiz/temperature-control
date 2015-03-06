#! /usr/bin/env python
from flask import Flask
from flask import jsonify
from flask import request
from flask import Response

from thermostat.interface import TemperatureControl


app = Flask(__name__)


@app.route("/temperature/", methods=["GET", "POST"])
def temperature():
  if request.method == "GET":
    temperature = temperature_control.get_temperature()
    response = jsonify(temperature=temperature)

  else:
    data = request.get_json()
    temperature = data['temperature']
    temperature_control.set_temperature(temperature)
    response = Response(status=204)

  return response


@app.route("/status/", methods=["GET", "POST"])
def status():
  if request.method == "GET":
    is_active = temperature_control.is_active()
    status = temperature_control.get_status()
    response = jsonify(is_active=is_active, status=status)

  else:
    data = request.get_json()
    is_active = data['is_active']
    if is_active:
      temperature_control.switch_on()
    else:
      temperature_control.switch_off()
    response = Response(status=204)

  return response


if __name__ == "__main__":
  # Connect to Arduino
  temperature_control = TemperatureControl.autoconnect()

  # Start server. See http://localtunnel.me/
  app.run(host='0.0.0.0', debug=True)
