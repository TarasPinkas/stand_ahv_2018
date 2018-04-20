#include <EtherCard.h>
#include <string.h>

#define MOTOR_PIN 3    // motor

/* distance sensor (HRC) */
#define TRIGER_PIN 5   // HRC 5v
#define ECHO_PIN 6     // HRC 5v

#define TERM_PIN 1    // temperature sensor
#define DAMP_PIN 0      // humidity sensor

#define LOW_HUMIDITY 50
#define MAX_LEVEL 16
#define MIN_WATER_LEVEL 4
#define DATA_LEN 40
#define TIMER_DELAY 5000 // msecs


static unsigned long timer = 0;
static bool motor_state = 0;

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x71,0x69,0x69,0x2D,0x30,0x31 };
const char website[] PROGMEM = "www.tobo.com.ua";
byte Ethernet::buffer[700];


int get_distance(int t_pin, int e_pin);
void set_motor_state(int m_pin, bool is_on);
int get_humidity(int t_pin);
int get_celsius(int t_pin);
static void my_callback (byte status, word off, word len);
bool setup_eth(const byte *mac, const char *site);
bool eth_func(const char *site);

void setup()
{

  Serial.begin(9600);

  Serial.println("start setup");

  pinMode(DAMP_PIN, INPUT);
  analogWrite(TERM_PIN, LOW);
 

  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(TRIGER_PIN, OUTPUT);
  pinMode(TERM_PIN, INPUT);

  set_motor_state(MOTOR_PIN, false);

  setup_eth(mymac, website);
}


void loop()
{
  int res = 0;
  ether.packetLoop(ether.packetReceive());

  if (millis() > timer)
  {
    timer = millis() + TIMER_DELAY;
    Serial.println();
    Serial.print("<<< REQ ");
    eth_func(website);
  }
  else {
      if (get_humidity(DAMP_PIN) < LOW_HUMIDITY && get_distance(TRIGER_PIN, ECHO_PIN) > MIN_WATER_LEVEL)
   set_motor_state(MOTOR_PIN, true);
  else
    set_motor_state(MOTOR_PIN, false);
    }





  delay(500);
}


bool eth_func(const char *site)
{
  char dataToBdCh[DATA_LEN] = { 0 };
  unsigned temp = get_celsius(TERM_PIN);
  unsigned level = get_distance(TRIGER_PIN, ECHO_PIN);
  unsigned humidity = get_humidity(DAMP_PIN);
  bool state = (motor_state);
  
  if (snprintf(dataToBdCh, DATA_LEN, "?%s%2d&%s%2d&%s%2d&%s%2d",
        "moisture=", humidity, "pump=", state,
        "temp=", temp, "level=", level) < 0)
  {
    Serial.println("Failed to prepare the request");
    return false;
  }

  ether.browseUrl(PSTR("/test.php"), dataToBdCh, site, my_callback);
  return true;
}

// called when the client request is complete
static void my_callback (byte status, word off, word len)
{
  Serial.println(">>>");

  Ethernet::buffer[off+300] = 0;

  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
}

bool setup_eth(const byte *mac, const char *site)
{
  if (ether.begin(sizeof Ethernet::buffer, mac) == 0)
  {
    Serial.println(F("Failed to access Ethernet controller"));
    return false;
  }

  if (!ether.dhcpSetup())
  {
    Serial.println(F("DHCP failed"));
    return false;
  }

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  if (!ether.dnsLookup(site))
  {
    Serial.println("DNS failed");
    return false;
  }

  ether.printIp("SRV: ", ether.hisip);

  return true;
}

int get_distance(int t_pin, int e_pin)
{
  digitalWrite(t_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(t_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(t_pin, LOW);

  int dist = pulseIn(e_pin, HIGH);

  dist = ((dist / 2) / 29.1);
  
  if(MAX_LEVEL - dist < 0){
    return 0;
  }
  else {
    if(MAX_LEVEL - dist > 16) {
      return 16;
    } else {
        return MAX_LEVEL - dist;
    }
  }
}

void set_motor_state(int m_pin, bool is_on)
{
  if (motor_state == is_on)
    return;
Serial.print("mtor state = ");
Serial.print(motor_state);
Serial.print("\tnew = ");
Serial.println(is_on);
  digitalWrite(m_pin, is_on ? 255 : 0);
  motor_state = is_on;
}

int get_humidity(int t_pin)
{

   int moisture = analogRead(t_pin);
   if(moisture < 470) {moisture = 100;}
   else if(moisture > 1016) {moisture = 31;}
   else {moisture = (1016-moisture)*0.1264+31;}
  return moisture;
}

int get_celsius(int t_pin)
{
  float temp = analogRead(t_pin);
  float mv = (temp / 1024.0) * 5000;
  Serial.println(mv/10);
  return (mv / 10);
}


