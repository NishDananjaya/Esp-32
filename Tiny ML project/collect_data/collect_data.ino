#include <WiFi.h>
#include "SPIFFS.h"
#include <WebServer.h>

const char* ssid = "Nish_DRouter";
const char* password = "Nishan622";
const int MAX_SAMPLES = 1000;  // Adjust based on your needs
const String FILENAME = "/rssi_data.csv";
WebServer server(80);

int sampleCount = 0;

void setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)) {
    Serial.println("SPIFFS initialization failed!");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Setup download endpoint
  server.on("/download", HTTP_GET, []() {
    File file = SPIFFS.open(FILENAME, "r");
    if(!file) {
      server.send(500, "text/plain", "Failed to open file");
      return;
    }
    server.streamFile(file, "text/csv");
    file.close();
  });
  
  server.begin();
  Serial.print("\nUse this URL to download data: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/download");

  // Initialize file with header
  File file = SPIFFS.open(FILENAME, "w");
  if(file) {
    file.println("timestamp,rssi");
    file.close();
  }
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    int rssi = WiFi.RSSI();
    unsigned long timestamp = millis();
    
    // Rotate data if needed
    if(sampleCount >= MAX_SAMPLES) {
      rotateDataFile();
      sampleCount = 0;
    }
    
    // Append new data
    File file = SPIFFS.open(FILENAME, "a");
    if(file) {
      file.print(timestamp);
      file.print(",");
      file.println(rssi);
      file.close();
      sampleCount++;
      
      // Print to Serial Monitor
      Serial.print(timestamp);
      Serial.print(",");
      Serial.println(rssi);
    }
  } else {
    Serial.println("WiFi disconnected! Reconnecting...");
    WiFi.reconnect();
  }
  
  server.handleClient();  // Handle web server requests
  delay(1000);
}

void rotateDataFile() {
  File file = SPIFFS.open(FILENAME, "r");
  if(!file) return;
  
  // Skip header
  String header = file.readStringUntil('\n');
  
  // Read last MAX_SAMPLES/2 lines
  String buffer = "";
  while(file.available()) {
    String line = file.readStringUntil('\n');
    buffer += line + "\n";
  }
  file.close();
  
  // Write back header and last portion of data
  file = SPIFFS.open(FILENAME, "w");
  if(file) {
    file.println(header);
    file.print(buffer);
    file.close();
  }
}