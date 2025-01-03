#include <Arduino.h>

const int ledPin = 2; // GPIO pin where the LED is connected

void setup() {
    pinMode(ledPin, OUTPUT); // Initialize the LED pin as an output
}

void loop() {
    digitalWrite(ledPin, HIGH); // Turn the LED on
    delay(1000);                // Wait for a second
    digitalWrite(ledPin, LOW);  // Turn the LED off
    delay(1000);                // Wait for a second
}
