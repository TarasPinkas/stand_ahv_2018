#define TERM_PIN 6


void setup() {
  Serial.begin(9600);
  pinMode(TERM_PIN, INPUT);
}

void loop() {
 float temp = analogRead(TERM_PIN);
 float mv = (temp / 1024.0) * 5000;
 float cel = (mv / 10);

 Serial.print("Temp = ");
 Serial.println(cel);

 delay(1000);
}
