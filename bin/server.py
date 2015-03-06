#! /usr/bin/env python
from flask import Flask
from flask import jsonify
from flask import Response

from temperature_control import TemperatureControl


app = Flask(__name__)


@app.route("/get-temperature/")
def get_temperature():
  temperature = temperature_control.get_temperature()
  return jsonify(temperature=temperature)


@app.route("/set-temperature/<float:temperature>", methods=["POST"])
def set_temperature(temperature):
  temperature_control.set_temperature(temperature)
  return Response(status=204)


if __name__ == "__main__":
  # Connect to Arduino
  temperature_control = TemperatureControl.autoconnect()

  # Start server. See http://localtunnel.me/
  app.run(host='0.0.0.0', debug=True)
