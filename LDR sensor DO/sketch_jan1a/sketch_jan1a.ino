#define LDR_PIN 5 // Digital pin connected to DO pin of the LDR module

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  int ldrState = digitalRead(LDR_PIN); // Read the digital output from the LDR module

  if (ldrState == HIGH) {
    Serial.println("Light Detected");
  } else {
    Serial.println("Darkness Detected");
  }

  delay(500); // Delay for stability
}
