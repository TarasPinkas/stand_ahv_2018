#define MOTOR_PIN 3

void set_motor_state(bool is_on)
{

  Serial.print("Motor state = ");
  Serial.println(is_on);

  analogWrite(MOTOR_PIN, is_on ? 0 : 255);
}

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_PIN, OUTPUT);
}

void loop() {

  set_motor_state(1);

  delay(2000);

  set_motor_state(0);

  delay(2000);

}
