#define IR_SENSOR_PIN 27
#define ledPin 13


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(ledPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = digitalRead(IR_SENSOR_PIN);
  if (sensorValue == 0) {
    digitalWrite(ledPin, HIGH);
    Serial.println("object is present");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("object is absent");
  }
  delay(500);

}
