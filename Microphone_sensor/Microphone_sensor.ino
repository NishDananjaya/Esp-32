const int micPin = 4;
const int ledPin = 5;

void setup() {
  // put your setup code here, to run once:

  pinMode(micPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

  int micValue = digitalRead(micPin);

  if (micValue == HIGH) {
    Serial.println("Sound detected");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("No sound");
    digitalWrite(ledPin, LOW);
  }
  delay(500);

}
