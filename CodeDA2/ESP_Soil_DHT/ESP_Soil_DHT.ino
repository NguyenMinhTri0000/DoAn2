#include "DHT.h"

const int DHTPIN = D6;
const int DHTTYPE = DHT11;

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode (2, INPUT);

}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(t) || isnan(h)) {
    Serial.println(" Failed to read from DHT");
  } else {
    Serial.print("Nhiet do: ");
    Serial.println(t);
    Serial.print("Do am: ");
    Serial.println(h);
  }

  Serial.println();
  delay(1000);

}
