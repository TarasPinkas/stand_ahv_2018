#include <EtherCard.h>
#include <string.h>

#define DATA_LEN 40
#define TIMER_DELAY 5000 // msecs

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];
static unsigned long timer = 0;

const char website[] PROGMEM = "www.tobo.com.ua";

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



bool eth_func(const char *site)
{
	char dataToBdCh[DATA_LEN] = { 0 };

	if (snprintf(dataToBdCh, DATA_LEN, "?%s%2d&%s%2d&%s%2d&%s%2d",
				"moisture=", 1, "pump=", 3, "temp=", 4, "level=", 6) < 0)
	{
		Serial.println("Failed to prepare the request");
		return false;
	}

	ether.browseUrl(PSTR("/test.php"), dataToBdCh, site, my_callback);
	return true;
}

void setup ()
{

	Serial.begin(57600);
	Serial.println(F("\n[webClient]"));

	if (!setup_eth(mymac, website))
		Serial.println("Failed to setup Ethernet module");

}

void loop ()
{
	ether.packetLoop(ether.packetReceive());

	if (millis() > timer)
	{
		timer = millis() + TIMER_DELAY;
		Serial.println();
		Serial.print("<<< REQ ");
		eth_func(website);
	}
}
