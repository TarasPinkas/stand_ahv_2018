#include "/home/user/project/stand_ahv_2018/src/include/HRC.h"
#define TRIGER_PIN 11
#define ECHO_PIN 12

static int distance(int t_pin, int e_pin)
{
  digitalWrite(t_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(t_pin, HIGH);
  delayMicroseconds(10);

  digitalWrite(t_pin, LOW);

  long dist = pulseIn(e_pin, HIGH);

  return ((dist / 2) / 29.1);
}

void setup()
{
  Serial.begin(9600);

  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGER_PIN, OUTPUT);
  
}

void loop() {

  Serial.print("distance = ");
  Serial.println(distance(TRIGER_PIN, ECHO_PIN));
  delay(500);

}
