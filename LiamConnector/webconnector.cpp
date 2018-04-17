// 
// 
// 

#include <ESP8266WebServer.h>
#include "webconnector.h"


WebConnector::WebConnector(LiamState* liamState){
	server = ESP8266WebServer(80);
	state = liamState;
}

void WebConnector::setup(int port) {
	server.on("/", std::bind(&WebConnector::handleIndexRequest, this));
	server.begin();
	Serial.print("Use this URL to connect: ");
	Serial.print("http://");
	Serial.print(WiFi.localIP());
	Serial.println("/");

}

void WebConnector::handleIndexRequest() {
	Serial.println("Incoming request");
	String response = "Hello lawn";
	response += "\nSensor0:";
	response += state->Sensor0;
	response += "\nSensor1:";
	response += state->Sensor1;
	server.send(200, "text/plain", response);
}

void WebConnector::handleLoop() {
	server.handleClient();
}
