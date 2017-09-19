#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "myprojectdb-af8dd.firebaseio.com"
#define FIREBASE_AUTH "g8xy5sX81rk1gvdYr9v0Pi8QtdfiS9KOWqUKCfL0"


const char* ssid = "Jaques";
const char* password = "Harien22";
const char* host = "Panic-Button";

void connectWifi ();
void beginOTA ();

void setup() {
  Serial.begin(115200);
  connectWifi ();
  beginOTA ();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);


}

void loop() {
  ArduinoOTA.handle();
}

void connectWifi () {
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void beginOTA () {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(host);

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
  });
  ArduinoOTA.onEnd([]() {
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void sendTemp (float temp) {
  // append a new value to /logs

  String name = Firebase.pushFloat("temperature", temp);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /temperature failed:");
      Serial.println(Firebase.error());
      return;
  }
  Serial.print("pushed: /temperature/");
  Serial.print(name);
  Serial.print(" : ");
  Serial.println(temp);
}
