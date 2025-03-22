#include <Keyboard.h>

const int colPin = 2;     // Column pin
const int rowPin1 = 5;    // First row pin
const int rowPin2 = 7;    // Second row pin

void setup() {
  pinMode(rowPin1, INPUT_PULLUP);
  pinMode(rowPin2, INPUT_PULLUP);
  pinMode(colPin, OUTPUT);
  
  // Set column high initially
  digitalWrite(colPin, HIGH);
  
  Keyboard.begin();
}

void loop() {
  // Activate column (set LOW to check buttons on this column)
  digitalWrite(colPin, LOW);
  
  // Small delay for electrical stability
  delayMicroseconds(10);
  
  // Check button 1
  if (digitalRead(rowPin1) == LOW) {
    Keyboard.print("a");
    
    // Wait for key release (with timeout)
    unsigned long startTime = millis();
    while (digitalRead(rowPin1) == LOW) {
      if (millis() - startTime > 1000) break; // Prevent getting stuck
    }
    delay(50); // Debounce delay
  }
  
  // Check button 2
  if (digitalRead(rowPin2) == LOW) {
    Keyboard.print("1");
    
    // Wait for key release (with timeout)
    unsigned long startTime = millis();
    while (digitalRead(rowPin2) == LOW) {
      if (millis() - startTime > 1000) break; // Prevent getting stuck
    }
    delay(50); // Debounce delay
  }
  
  // Deactivate column
  digitalWrite(colPin, HIGH);
  
  // Small delay before next scan
  delay(10);
}