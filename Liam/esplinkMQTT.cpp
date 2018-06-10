#include <ELClient.h>
#include <ELClientCmd.h>
#include <ELClientMqtt.h>
#include <Arduino.h>
#include "Definition.h"

extern int command;
extern int state;

ELClient esp(&Serial, &Serial);
ELClientCmd cmd(&esp);
ELClientMqtt mqtt(&esp);
void mqttData(void *response)
{
  ELClientResponse *res = (ELClientResponse *)response;
  String topic = res->popString();
  String data = res->popString();
  Serial.print("MQTT In (");
  Serial.print(topic);
  Serial.print(") ");
  Serial.println(data);
  if(topic == F("/mower/1/command"))
  {
    if(data == "M")
      command = CMD_MOW;
    else if(data == "H")
      command = CMD_HOME;
    else
      command = CMD_AUTO;
  }
}
bool connected;
void mqttConnected(void *response)
{
  mqtt.subscribe("/mower/1/command");
  connected = true;
}
void mqttDisconnected(void *response)
{
  connected = false;
}

void mqtt_setup()
{
  bool ok;
  do
  {
    ok = esp.Sync();
    if(!ok) Serial.println(".");
  } while(!ok);
  Serial.println("Synced");
  mqtt.connectedCb.attach(mqttConnected);
  mqtt.disconnectedCb.attach(mqttDisconnected);
  mqtt.dataCb.attach(mqttData);
  mqtt.setup();
}

long last_mqtt;
void mqtt_send(int bat)
{
  esp.Process();
  if(!connected)
  {
    Serial.println("Not connected");
    return;
  }
  if((millis()-last_mqtt) < 10000)
  {
    Serial.println("Not time to send");
    return;
  }
  char buf[12];
  itoa(bat, buf, 10);
  mqtt.publish("/mower/1/battery", buf);
  Serial.print("Battery:");
  Serial.println(buf);
  itoa(state, buf, 10);
  mqtt.publish("/mower/1/state", buf);
  Serial.print("State:");
  Serial.println(buf);
  itoa(command, buf, 10);
  mqtt.publish("/mower/1/lastcmd", buf);
  Serial.print("Command:");
  Serial.println(buf);
  last_mqtt = millis();
}
