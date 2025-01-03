#include <WiFi.h>

const char* sta_ssid = "Nish_DRouter";
const char* sta_password = "Nishan622";
const char* ap_ssid = "ESP32";
const char* ap_password = "12345678";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(sta_ssid,sta_password);
  while(WiFi.status() !=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi....");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  WiFi.softAP(ap_ssid,ap_password);
  Serial.println("AP Started");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
