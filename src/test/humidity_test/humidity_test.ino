#define DAMP_PIN 3


void setup() {

  Serial.begin(9600);

  pinMode(DAMP_PIN, INPUT);
}

void loop() {
  
  Serial.print("Humidity = ");
  Serial.println(analogRead(DAMP_PIN));

  delay(500);
}
