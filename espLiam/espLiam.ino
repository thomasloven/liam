#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "XXXX";
const char* password = "XXXXXXXXXX";
IPAddress server(192,168,0,xxx);

WiFiClient wifiClient;
PubSubClient client(wifiClient);


void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("<");
  for(int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println(">");
}


void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(server, 1883);
  client.setCallback(callback);
}


char buffer[128];

void loop() {
  if(!client.connected())
  {
    reconnect();
  }
  client.loop();

  static int step = 0;
  while(Serial.available())
  {
    int len = Serial.readBytesUntil('\n', buffer, 128);
    buffer[len] = '\0';
    if(!strncmp(buffer, "BAT:", 4))
      client.publish("mower/1/battery", &buffer[4]);
    if(!strncmp(buffer, "STT:", 4))
      client.publish("mower/1/state", &buffer[4]);
    if(!strncmp(buffer, "CMD:", 4))
      client.publish("mower/1/lastcmd", &buffer[4]);
  }
}
void reconnect()
{
  while(!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if(client.connect("arduinoClient"))
    {
      Serial.println("connected");
      client.subscribe("mower/1/command");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
