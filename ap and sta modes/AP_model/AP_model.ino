#include <WiFi.h>

const char* ssid = "ESP32_AP";
const char* password = "123456789";

void setup(){
    Serial.begin(115200);
    WiFi.softAP(ssid, password);
    Serial.println("Access Point Started");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

}

void loop(){
    // Nothing here
}