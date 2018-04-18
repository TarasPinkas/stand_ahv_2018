#define DAMP_PIN 4      //
#define MOTOR_PIN 3    
#define TRIGER_PIN 11   // HRC 5v
#define ECHO_PIN 12     // HRC 5v
#define TERM_PIN 6

#define LOW_HUMIDITY 700

int max_level = 0;

int get_distance(int t_pin, int e_pin)
{
  digitalWrite(t_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(t_pin, HIGH);
  delayMicroseconds(10);

  digitalWrite(t_pin, LOW);

  int dist = pulseIn(e_pin, HIGH);

  return ((dist / 2) / 29.1);
}

void set_motor_state(bool is_on)
{
  static bool state = 0;

  if (state == is_on)
    return;
    
  digitalWrite(MOTOR_PIN, is_on ? 255 : 0);
  state = is_on;
  Serial.print("Motor state = ");
  Serial.println(is_on);
  
  delay(100);
}

int get_humidity(int t_pin)
{
  /*
   * need to add recalculation to %
   */
  return analogRead(t_pin);  
}

int get_celsius(int t_pin)
{
  float temp = analogRead(t_pin);
  float mv = (temp / 1024.0) * 5000;

  return (mv / 10);
}

void setup() {

  Serial.begin(9600);
Serial.println("start setup");

  pinMode(DAMP_PIN, INPUT);  
  
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  pinMode(TRIGER_PIN, OUTPUT);
  pinMode(TERM_PIN, INPUT);
  
  set_motor_state(false);

  max_level = get_distance(TRIGER_PIN, ECHO_PIN);
  max_level = max_level - 2;
}


void loop() 
{
  int level = 0, celsius = 0;
  int humidity = get_humidity(DAMP_PIN);

  Serial.print("Humidity = ");
  Serial.println(humidity);
  
  if (humidity > LOW_HUMIDITY)
    set_motor_state(true);
  else
    set_motor_state(false);

  level = get_distance(TRIGER_PIN, ECHO_PIN);

  Serial.print("Level = ");
  Serial.println(level);
  
  if (level < max_level)
  {
    set_motor_state(false);
  }

  celsius = get_celsius(TERM_PIN);

  Serial.print("Temperature = ");
  Serial.println(celsius);

  delay(1000);
}



