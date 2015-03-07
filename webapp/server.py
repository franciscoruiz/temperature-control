#! /usr/bin/env python
import argparse
import json

from flask import Flask
from flask import jsonify
from flask import request
from flask import Response
from flask import send_file

from thermostat.interface import DeviceNotFoundException
from thermostat.interface import Thermostat
from thermostat.storage import Storage
from thermostat.testing_utils import MockThermostat


_TEMPERATURE_DB = "temperatures.db"


parser = argparse.ArgumentParser(description="Run the thermostat server.")

parser.add_argument(
  "--test",
  dest="test",
  action="store_true",
  default=False,
  help="Run in test mode, not requiring an Arduino device",
  )

parser.add_argument(
  "--allow-external",
  dest="allow_external",
  action="store_true",
  default=False,
  help="Make the server available externally",
  )

parser.add_argument(
  "--debug",
  dest="debug",
  action="store_true",
  default=False,
  help="Enable debug mode",
  )


app = Flask(__name__)


@app.route("/")
def index():
  return send_file("index.html")


@app.route("/temperature/", methods=["GET", "POST"])
def temperature():
  storage = Storage(_TEMPERATURE_DB)
  if request.method == "GET":
    temperature = storage.get_latest()
    temperature = temperature and temperature.temperature or None
    response = jsonify(
      temperature=temperature,
      target=thermostat.target_temperature,
      )

  else:
    data = request.get_json()
    temperature = data["target"]
    thermostat.set_temperature(temperature)
    response = Response(status=204)

  return response


@app.route("/status/", methods=["GET", "POST"])
def status():
  if request.method == "GET":
    is_active = thermostat.is_active()
    status = thermostat.get_status()
    response = jsonify(is_active=is_active, status=status)

  else:
    data = request.get_json()
    is_active = data["is_active"]
    if is_active:
      thermostat.switch_on()
    else:
      thermostat.switch_off()
    response = Response(status=204)

  return response


if __name__ == "__main__":
  args = parser.parse_args()

  # Connect to Arduino
  try:
    thermostat = Thermostat.autoconnect()
  except DeviceNotFoundException:
    if args.test:
      thermostat = MockThermostat()
    else:
      exit("Cannot find device. Is the Arduino connected?")

  # Set a default target temperature
  thermostat.set_temperature(24.0)

  # Start server. See http://localtunnel.me/
  if args.allow_external:
    host = "0.0.0.0"
  else:
    host = None

  try:
    app.run(host=host, debug=args.debug)
  except KeyboardInterrupt:
    exit('Stopping server\n')
