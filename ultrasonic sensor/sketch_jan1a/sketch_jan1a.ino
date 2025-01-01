#define echoPin 18
#define trigPin 5

long duration;
float distance;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Trigger the sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo signal with a timeout
  duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout (max ~500cm)

  if (duration == 0) {
    Serial.println("No echo detected or out of range.");
  } else {
    distance = (duration * 0.034) / 2;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(1000);
}
