#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
#define RELAY_PIN 10

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


float readTemperature(void)
{
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}


void switchRelayOn(void) {
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Switching relay on");
}


void switchRelayOff(void) {
  digitalWrite(RELAY_PIN, LOW);
  Serial.println("Switching relay off");
}


void setup(void)
{
  Serial.begin(9600);
  Serial.println("Starting up Temperature Monitor/Controller");

  sensors.begin(); // IC Default 9 bit. If you have troubles consider upping it 12. Ups the delay giving the IC more time to process the temperature measurement
  
  pinMode(RELAY_PIN, OUTPUT);
  switchRelayOff();
}


void loop(void)
{ 
  float temperature = readTemperature();
  Serial.println(temperature);
  
  if (temperature < 28.0) {
    switchRelayOn();
  } else {
    switchRelayOff();
  }
}

