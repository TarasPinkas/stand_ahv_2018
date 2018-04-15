#include "DHT.h"

/* Digital pins */
#define TERM_PIN  13

// temperature sensor
DHT *dht;

bool get_dht_data(DHT *dht, float *t, float *h)
{
  if (!dht || !t || !h)
    return false;

  *t = dht->readTemperature();
  *h = dht->readHumidity();

  if (isnan(*h) || isnan(*t))
    return false;

  return true;
}

void setup() {
  Serial.begin(9600);
  
/* 
 * create object from DHT22 sensor
 * if sensor DHT11 use:  DHT(TERM_PIN, DHT11)
 */
  dht = new DHT(TERM_PIN, DHT22);
  dht->begin();
  
}

float *t, *h;

void loop()
{
  if (!get_dht_data(dht, t, h))
  {
    Serial.println("Error");
  }

  Serial.print("Temperature = ");
  Serial.println(*t);
  Serial.print("Humidity = ");
  Serial.println(*h);

  delay(500);
}
