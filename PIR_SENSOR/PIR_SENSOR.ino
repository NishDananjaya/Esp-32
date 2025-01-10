const int PIR_SENSOR = 5; // PIR sensor pin
const int LED = 2;         // LED pin

unsigned long lastMotionTime = 0;    // Stores the last motion detection time
const unsigned long debounceDelay = 200; // Debounce delay (200 ms)
const unsigned long warmupTime = 10000;  // PIR warm-up time (10 seconds)

bool isWarmupComplete = false;       // Flag to indicate PIR warm-up status
unsigned long startTime;             // Tracks the time since startup

void setup() {
  Serial.begin(115200);              // Initialize serial communication
  pinMode(PIR_SENSOR, INPUT);        // Set PIR sensor pin as input
  pinMode(LED, OUTPUT);              // Set LED pin as output

  digitalWrite(LED, LOW);            // Ensure the LED starts off
  startTime = millis();              // Store the current time
  Serial.println("PIR sensor warming up...");
}

void loop() {
  // Handle PIR warm-up
  if (!isWarmupComplete) {
    if (millis() - startTime > warmupTime) {
      isWarmupComplete = true;
      Serial.println("PIR sensor is ready!");
    }
    return; // Exit loop during warm-up
  }

  // Read the PIR sensor
  int pirValue = digitalRead(PIR_SENSOR);

  // Motion detection with debounce
  if (pirValue == HIGH && (millis() - lastMotionTime > debounceDelay)) {
    Serial.println("Motion Detected");
    digitalWrite(LED, HIGH); // Turn on the LED
    lastMotionTime = millis();
  } else if (pirValue == LOW) {
    Serial.println("No motion detected");
    digitalWrite(LED, LOW); // Turn off the LED
  }

  delay(1000); // Wait 1 second before checking again
}
