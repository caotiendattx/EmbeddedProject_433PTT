#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"   //library for reading file
#include <ArduinoJson.h> //library for formating as JSON

AsyncWebServer server(80);
StaticJsonDocument<200> jsonBuffer;
String jsonString;

void setupAPMode() {
  const char* ssid = "433PTT"; // Set the name of the access point
  const char* password = "12345678"; // Set the password for the access point
  WiFi.softAP(ssid, password); // Set the ESP32 to AP mode with the specified SSID and password
  Serial.print("Access point IP address: ");
  Serial.println(WiFi.softAPIP()); // Print the IP address of the access point
}

void handleOnEndpoint(AsyncWebServerRequest *request) {
  jsonBuffer.clear();


  //call to embedded func()
  //return 20.02
  jsonBuffer["dbValue"] = "20.02";
  jsonBuffer["status"] = "success";

  String response;
  serializeJson(jsonBuffer, response);
  request->send(200, "application/json", response);
  jsonBuffer.clear();
}

void handleSendRF(AsyncWebServerRequest *request){
  Serial.println("handleSendRF ");
    String payload = request->getParam("key", true)->value();

    Serial.print("Received JSON data: ");
    Serial.println(payload);

    // AsyncResponseStream *response = request->beginResponseStream("application/json");
    // response->print("{\"status\":\"success\"}");
    String response = "post data success";
    request->send(200, "application/json",response);
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

  server.on("/on", HTTP_GET, handleOnEndpoint);
  server.on("/postDataRF", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.print("Content type::");
    Serial.println(request->contentType());
    Serial.println("OFF hit.");
    String message;
    int params = request->params();
    Serial.printf("%d params sent in\n", params);

    //send url as params such as /postDataRF?key="value"
    //p->name().c_str() = key
    //p->value().c_str() = value
    for (int i = 0; i < params; i++)
    {
        AsyncWebParameter *p = request->getParam(i);
        if (p->isFile())
        {
            Serial.printf("_FILE[%s]: %s, size: %u", p->name().c_str(), p->value().c_str(), p->size());
        }
        else if (p->isPost())
        {
            Serial.printf("%s: %s \n", p->name().c_str(), p->value().c_str());
        }
        else
        {
            Serial.printf("_GET[%s]: %s", p->name().c_str(), p->value().c_str());
        }
    }
    request->send(200, "text/plain", "post request successfully");
  });
  server.begin();
}

void setup()
{
  Serial.begin(9600); //define frequency of serial monitor
  Serial.println("successfully");
  setupAPMode();
  setUpRoutes();
  delay(1000);
}

void loop()
{
}
