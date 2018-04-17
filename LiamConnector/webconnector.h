// webserver.h

#ifndef _WEBCONNECTOR_h
#define _WEBCONNECTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LiamState.h"

class WebConnector {


public:

	WebConnector(LiamState* liamState);
	void setup(int port);
	void handleIndexRequest();
	void handleLoop();

private:
	ESP8266WebServer server;
	LiamState* state;
};

#endif


