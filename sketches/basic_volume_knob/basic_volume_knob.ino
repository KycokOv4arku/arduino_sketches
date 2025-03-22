// simplest thing from tutorial https://docs.arduino.cc/built-in-examples/basics/AnalogReadSerial/
// 3 pins. GND, 5v and wiper to A0 (any anlog pin)

#include <HID-Project.h>

const int potPin = A0;      // Potentiometer connected to A0
int lastPotValue = 0;       // Last read value
int threshold = 1;          // Minimum change required to send a command

void setup() {
  Consumer.begin();         // Initialize consumer control (media keys)
  pinMode(potPin, INPUT);   // Set up potentiometer pin
  Serial.begin(9600);       // For debugging (optional)
}

void loop() {
  int potValue = analogRead(potPin);

  if (abs(potValue - lastPotValue) > threshold) {
    int desiredVolume = map(potValue, 0, 1023, 0, 100);
    int volumeDiff = desiredVolume - map(lastPotValue, 0, 1023, 0, 100);

    int steps = (abs(volumeDiff) + 1) / 2; // Calculate steps, round up
    auto command = (volumeDiff > 0) ? MEDIA_VOLUME_UP : MEDIA_VOLUME_DOWN;

    for (int i = 0; i < steps; i++) {
      Consumer.write(command);
      delay(5);
    }

    Serial.println(volumeDiff > 0 ? "Volume UP" : "Volume DOWN");
    lastPotValue = potValue;
    delay(50);
  }

  delay(10);
}
