import argparse
import time

from thermostat.interface import Thermostat
from thermostat.storage import Storage
from thermostat.testing_utils import MockThermostat


parser = argparse.ArgumentParser(description="Run the thermostat logger.")

parser.add_argument(
  "database",
  help="DB to store temperatures",
  )

parser.add_argument(
  "seconds",
  type=int,
  help="Polling frequency in seconds",
  )

parser.add_argument(
  "--init-db",
  dest="init_db",
  action="store_true",
  default=False,
  help="Create DB schema",
  )

parser.add_argument(
  "--test",
  dest="test",
  action="store_true",
  default=False,
  help="Run in test mode, not requiring an Arduino device",
  )


if __name__ == "__main__":
  args = parser.parse_args()

  if args.test:
    thermostat = MockThermostat()
  else:
    thermostat = Thermostat.autoconnect()

  storage = Storage(args.database)
  if args.init_db:
    storage.create_schema()

  try:
    while True:
      temperature = thermostat.get_temperature()
      storage.insert(temperature)
      time.sleep(args.seconds)
  except KeyboardInterrupt:
    storage.close()
    exit('Stopping logger\n')

  main()
