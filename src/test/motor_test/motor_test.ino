#define MOTOR_PIN 3


void set_motor_state(bool is_on)
{
  static bool state = 0;
 

  if (state == is_on)
    return;
    
  Serial.print("Motor state = ");
  Serial.println(is_on);
  
  digitalWrite(MOTOR_PIN, is_on ? 255 : 0);
  state = is_on;
}

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_PIN, OUTPUT);

  
}

void loop() {

  set_motor_state(1);
  delay(400);
  
  set_motor_state(1);
  delay(400);
   set_motor_state(1);
  delay(400);
  
  set_motor_state(0);
  delay(400);
  set_motor_state(0);
  delay(400);
  set_motor_state(0);
  delay(400);

}
