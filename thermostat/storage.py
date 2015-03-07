from collections import namedtuple
from datetime import datetime
import sqlite3


Temperature = namedtuple("Temperature", ["timestamp", "temperature"])


class Storage(object):

  def __init__(self, database):
    self._conn = sqlite3.connect(database)
    self._conn.row_factory = _temperature_row_factory

  def create_schema(self):
    cursor = self._conn.cursor()
    cursor.execute("""
      CREATE TABLE temperatures (
        timestamp DATETIME,
        temperature REAL
      )""")
    self._conn.commit()

  def insert(self, temperature):
    cursor = self._conn.cursor()
    cursor.execute(
      "INSERT INTO temperatures (timestamp, temperature) VALUES (?, ?)",
      (datetime.now(), temperature),
      )
    self._conn.commit()

  def get_latest(self):
    return self.temperatures().fetchone()

  def temperatures(self):
    cursor = self._conn.cursor()
    cursor.execute("""
      SELECT DATETIME(timestamp), temperature
      FROM temperatures
      ORDER BY timestamp DESC
      """)
    return cursor

  def close(self):
    self._conn.close()


def _temperature_row_factory(cursor, row):
  timestamp = datetime.strptime(row[0], '%Y-%m-%d %H:%M:%S')
  return Temperature(timestamp=timestamp, temperature=row[1])
