#include <ESP8266WebServer.h>
#include "LiamState.h"
#include "webconnector.h"
#include <WiFiUdp.h>
#include <WiFiServer.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFi.h>

LiamState liamState;
WebConnector webconnector(&liamState);


void setup() {
	Serial.begin(115200);
	delay(100);

	setupWifi("__ssid___", "__pass__");
}

void setupWifi(char* ssid, char* password) {

	WiFi.mode(WIFI_STA);
	WiFi.hostname("liamconnector");
	WiFi.begin(ssid, password);

	long t = millis();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
		if (t + 10000 < millis()) {
			break;
		}
	}

	if (WiFi.status() == WL_CONNECTED) {
		Serial.println("");
		Serial.println("WiFi connected");
	}
	else {
		Serial.println("");
		Serial.println("WiFi connect failed. Setting up AP");

		IPAddress    apIP(42, 42, 42, 42);  // Defining a static IP address: local & gateway
											// Default IP in AP mode is 192.168.4.1

		WiFi.mode(WIFI_AP_STA);
		WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00

																	  /* You can remove the password parameter if you want the AP to be open. */
		WiFi.softAP("liamconn__use_42.42.42.42");

		IPAddress myIP = WiFi.softAPIP();
		Serial.print("AP IP address: ");
		Serial.println(myIP);
	}

	webconnector.setup(80);
}

// the loop function runs over and over again until power down or reset
void loop() {
	webconnector.handleLoop();
	if ((millis() / 1000) % 2 == 0) {
		liamState.Sensor0 = OutOfBounds;
	}
	else {
		liamState.Sensor0 = InsideBounds;
	}
	delay(100);
}
