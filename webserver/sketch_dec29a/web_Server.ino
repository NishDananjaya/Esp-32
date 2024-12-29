//Load WiFi library
#include <WiFi.h>

const char* ssid = "Nish_DRouter";
const char* password = "Nishan622";

WiFiServer server(80);

String header;

String output26state = "off";
String output27state = "off";

const int output26 = 26;
const int output27 = 27;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
    Serial.begin(115200);
    pinMode(26, OUTPUT);
    pinMode(27, OUTPUT);

    digitalWrite(26, LOW);
    digitalWrite(27, LOW);

    Serial.println("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop() {
    WiFiClient client = server.available();

    if (client) {
        currentTime = millis();
        previousTime = currentTime;
        Serial.println("New Client.");
        String currentLine = "";
        while (client.connected() && currentTime - previousTime <= timeoutTime) {
            currentTime = millis();
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                header += c;
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();

                        if (header.indexOf("GET /26/on") >= 0) {
                            Serial.println("GPIO 26 on");
                            output26state = "on";
                            digitalWrite(26, HIGH);
                        } else if (header.indexOf("GET /26/off") >= 0) {
                            Serial.println("GPIO 26 off");
                            output26state = "off";
                            digitalWrite(26, LOW);
                        } else if (header.indexOf("GET /27/on") >= 0) {
                            Serial.println("GPIO 27 on");
                            output27state = "on";
                            digitalWrite(27, HIGH);
                        } else if (header.indexOf("GET /27/off") >= 0) {
                            Serial.println("GPIO 27 off");
                            output27state = "off";
                            digitalWrite(27, LOW);
                        }

                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css\">");
                        client.println("<style>");
                        client.println("body { font-family: Arial, sans-serif; margin: 0; text-align: center; background: #f4f4f9; color: #333; }");
                        client.println("h1 { background: #007bff; color: white; padding: 10px 0; margin: 0; }");
                        client.println(".container { padding: 20px; }");
                        client.println(".button { background-color: #007bff; border: none; color: white; padding: 15px 30px; text-decoration: none; font-size: 18px; margin: 10px; border-radius: 5px; cursor: pointer; }");
                        client.println(".button:hover { background-color: #0056b3; }");
                        client.println(".button.off { background-color: #6c757d; }");
                        client.println(".button.off:hover { background-color: #5a6268; }");
                        client.println(".status { font-size: 20px; margin: 20px 0; }");
                        client.println("</style></head>");
                        client.println("<body>");
                        client.println("<h1>ESP32 Web Server</h1>");
                        client.println("<div class=\"container\">");
                        client.println("<div class=\"status\">GPIO 26 - State: " + output26state + "</div>");
                        client.println("<a href=\"/26/on\" class=\"button\">Turn ON</a>");
                        client.println("<a href=\"/26/off\" class=\"button off\">Turn OFF</a>");
                        client.println("<div class=\"status\">GPIO 27 - State: " + output27state + "</div>");
                        client.println("<a href=\"/27/on\" class=\"button\">Turn ON</a>");
                        client.println("<a href=\"/27/off\" class=\"button off\">Turn OFF</a>");
                        client.println("</div>");
                        client.println("</body></html>");

                        client.println();
                        break;
                    } else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }
            }
        }
        header = "";
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}
