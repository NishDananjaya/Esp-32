#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "Nish_DRouter";
const char* password = "Nishan622";

// DHT Sensor
DHT dht(26, DHT11);
WebServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Handle root URL
  server.on("/", handleRoot);
  
  // Handle data request
  server.on("/readData", handleReadData);
  
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = R"(
    <!DOCTYPE html>
    <html>
    <head>
        <meta name='viewport' content='width=device-width, initial-scale=1.0'>
        <title>ESP32 Weather Station</title>
        <style>
            body {
                font-family: Arial, sans-serif;
                background: linear-gradient(135deg, #1e4d92, #1ca8dd);
                margin: 0;
                padding: 20px;
                min-height: 100vh;
                display: flex;
                justify-content: center;
                align-items: center;
            }
            .container {
                background: rgba(255, 255, 255, 0.9);
                border-radius: 15px;
                padding: 20px;
                box-shadow: 0 8px 32px rgba(0,0,0,0.1);
                text-align: center;
                max-width: 400px;
                width: 100%;
            }
            h1 {
                color: #2c3e50;
                margin-bottom: 30px;
            }
            .reading {
                background: #fff;
                border-radius: 10px;
                padding: 15px;
                margin: 10px 0;
                box-shadow: 0 4px 6px rgba(0,0,0,0.1);
            }
            .value {
                font-size: 2em;
                font-weight: bold;
                color: #3498db;
                margin: 10px 0;
            }
            .unit {
                color: #7f8c8d;
                font-size: 0.9em;
            }
        </style>
    </head>
    <body>
        <div class='container'>
            <h1>Weather Station</h1>
            <div class='reading'>
                <div>Temperature</div>
                <div class='value' id='temp'>--</div>
                <div class='unit'>°C</div>
            </div>
            <div class='reading'>
                <div>Humidity</div>
                <div class='value' id='hum'>--</div>
                <div class='unit'>%</div>
            </div>
        </div>
        <script>
            function updateReadings() {
                fetch('/readData')
                    .then(response => response.json())
                    .then(data => {
                        document.getElementById('temp').textContent = data.temperature;
                        document.getElementById('hum').textContent = data.humidity;
                    });
            }
            setInterval(updateReadings, 2000);
            updateReadings();
        </script>
    </body>
    </html>
  )";
  server.send(200, "text/html", html);
}

void handleReadData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  String data = "{\"temperature\":" + String(temperature, 1) + 
                ",\"humidity\":" + String(humidity, 1) + "}";
                
  server.send(200, "application/json", data);
}