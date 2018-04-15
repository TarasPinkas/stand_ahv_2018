#include "/home/user/project/stand_ahv_2018/src/include/HRC.h"

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

int get_distance(int t_pin, int e_pin)
{
	return distance(t_pin, e_pin);
}
