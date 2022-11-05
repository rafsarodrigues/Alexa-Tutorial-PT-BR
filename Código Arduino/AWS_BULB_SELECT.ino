#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(256);

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".wifi");
  }

  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println(", failed to connect to WiFi");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(AWS_IOT_ENDPOINT, 8883, net);
  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(THINGNAME)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(AWS_IOT_PUBLISH_TOPIC, "Reconnected");
      // ... and resubscribe
      client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setupBulb()
{
  pinMode(14, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT); 
}

void publishReturnMessage()
{
  StaticJsonDocument<200> doc;
  doc["finish"] = "OK_ALEXA";
  doc["language"] = "en-US";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("Log incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
    String sequence = doc["sequence"];
    char str_array[sequence.length()];
    sequence.toCharArray(str_array, sequence.length());
    const String location = doc["location"];
    const int delayBulb = doc["delay"];
    byte index = 0;
    uint8_t strings[128]; // an array of pointers to the pieces of the above array after strtok()
    char *ptr = NULL;
    ptr = strtok(str_array, ",");  // delimiter
   while (ptr != NULL)
   {
      strings[index] = (uint8_t)atoi(ptr);
      index++;
      ptr = strtok(NULL, ",");
   }
   Serial.println("The Pieces separated by strtok()");
   for (int n = 0; n < index; n++)
   {
      Serial.print(n);
      Serial.print("  ");
      Serial.println(strings[n]);
      digitalWrite(strings[n], HIGH);
      delay(delayBulb);
      digitalWrite(strings[n], LOW);
      delay(100);
   }
   publishReturnMessage();
}

void setup() {
  Serial.begin(9600);
  connectAWS();
  setupBulb();
}

void loop() {
  if (!client.connected()) {reconnect();}
  client.loop();
  delay(1000);
}
