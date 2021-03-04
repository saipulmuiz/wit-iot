#define SerialMon Serial

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>     // Include the Wi-Fi library
#include <PubSubClient.h>
#include <ArduinoJson.h>
//#include "DHTesp.h"
#include "DHT.h"


//Create software serial object to communicate with SIM800L
WiFiClient espClient;
PubSubClient mqtt(espClient);
const char* ssid     = "FoXiFi";     // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "saipul123258";     // The password of the Wi-Fi network
const char* broker = "192.168.43.15";     // ip / domain your mqtt broker example (192.168.1.2)
const char* deviceName = "pompa";      // name of your device
StaticJsonDocument<250> wrapper;
//DHTesp dht;
#define dhtPin 16
#define ad DHT22
DHT dht(dhtPin, ad);
boolean res;
boolean mqttConnect() {
  char buffer[256];
  SerialMon.print("Connecting to ");
  SerialMon.print(broker);
  wrapper["deviceName"] = deviceName;

  // Connect to MQTT Broker
  boolean status = mqtt.connect(deviceName);

  if (status == false) {
    SerialMon.println(" fail");
    return false;
  }
  SerialMon.println(" success");
  mqtt.subscribe("pompa");
  mqtt.subscribe("pompa-stats");
  wrapper["kind"] = "connected";
  wrapper["status"] = true;
  size_t n = serializeJson(wrapper, buffer);
  mqtt.publish("report", buffer, n);
  return mqtt.connected();
}
void callback(char* topic, byte* payload, unsigned int length);

void setup()
{
  ESP.eraseConfig();
  SerialMon.begin(9600);
  dht.begin();
  WiFi.begin(ssid, password);
//  dht.setup(D2, DHTesp::DHT22); // Connect DHT sensor to GPIO 17
  Serial.print("Connecting to ");
  pinMode(D1, OUTPUT); // initialize pin as OUTPUT
  //  digitalWrite(D1,0);
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print('*');
  }


  mqtt.setServer(broker, 1883); // connect to mqtt broker with port (default : 1883)
  mqtt.setCallback(callback);

}

void loop()
{

  if (!mqtt.connected()) {
    SerialMon.println("Trying Connecting to mqtt broker");
    if (mqttConnect()) {
      SerialMon.println("MQTT Connected");
    }
  }

  mqtt.loop();
//  SerialMon.println(dht.getHumidity());
//  SerialMon.println(dht.getTemperature());
 SerialMon.println(dht.readHumidity());
  SerialMon.println(dht.readTemperature());
  delay(1000);

}

void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  char buffer[256];
  doc["deviceName"] = deviceName;
  doc["kind"] = topic;

  if (strcmp(topic, "pompa") == 0) {
    if (payload[0] == '1') {
      digitalWrite(D1, 0);
      doc["status"] = true;
      SerialMon.println("Pompa Nyala");
    }
    if (payload[0] == '0') {
      digitalWrite(D1, 1);
      doc["status"] = false;
      SerialMon.println("Pompa Mati");
    }
    size_t n = serializeJson(doc, buffer);
    mqtt.publish("pompa-report", buffer, n);

  } else if (strcmp(topic, "pompa-stats") == 0) {
    if (payload[0] == '1') {
//      delay(dht.getMinimumSamplingPeriod());
//      delay(1000);
//      float humidity = dht.getHumidity();
//      float temperature = dht.getTemperature();
//      SerialMon.println("Get Data");
//      SerialMon.println(humidity);
//      SerialMon.println(temperature);
//      delay(1000);
//
//      doc["humidity"] = humidity;
//      doc["temperature"] = temperature;
//
//      size_t n = serializeJson(doc, buffer);
//      mqtt.publish("pompa-report", buffer, n);
    }
  }
  doc.clear();
}
