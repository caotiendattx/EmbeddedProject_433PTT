#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"   //library for reading file
#include <ArduinoJson.h> //library for formating as JSON - version 5.13

AsyncWebServer server(80);
StaticJsonBuffer<200> jsonBuffer;
String jsonString;

void setupAPMode() {
  const char* ssid = "433PTT"; // Set the name of the access point
  const char* password = "12345678"; // Set the password for the access point
  WiFi.softAP(ssid, password); // Set the ESP32 to AP mode with the specified SSID and password
  Serial.print("Access point IP address: ");
  Serial.println(WiFi.softAPIP()); // Print the IP address of the access point
}

void handleGetView(AsyncWebServerRequest *request) {
  jsonBuffer.clear();
  JsonObject& object = jsonBuffer.createObject();
  // {
  //   "hello": "world";
  // }
  object["hello"] = "world";
  String response;
  object.printTo(response);
  request->send(200, "application/json", response);
}

// example data to post:
// {
//   "key1": "value1",
//   "key2": "value2"
// }
void handlePostBodySend(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
      DynamicJsonBuffer jsonDynamicBuffer;
      JsonObject& root = jsonDynamicBuffer.parseObject((const char*)data);
      if (root.success()) {
        if (root.containsKey("key1")) {
          Serial.println(root["key"].asString());
        }
        if (root.containsKey("key2")) {
          Serial.println(root["key2"].asString());
        }
        request->send(200, "text/plain", "");
      } else {
        request->send(404, "text/plain", "");
      }
  }

void setUpRoutes(){
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.html", String(), false);
  });
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/styles.css", "text/css");
  });
  server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.js", "application/javascript");
  });

  server.on("/on", HTTP_GET, handleGetView);

  server.on("/postDataRF", HTTP_POST, [](AsyncWebServerRequest *request){},NULL, handlePostBodySend);

  server.begin();
}

void setup()
{
  Serial.begin(9600); //define frequency of serial monitor
  Serial.println("successfully");
  setupAPMode();
  setUpRoutes();
}

void loop()
{
}
