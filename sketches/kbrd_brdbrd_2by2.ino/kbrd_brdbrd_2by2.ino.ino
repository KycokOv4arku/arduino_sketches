// with GPT expanded to 2x2 from 2x1 - kbrd_brdbrd_2_1.ino 
// r0c0 - win+C -> VScode
// r0c1 - win+Q -> telegram
// r1c1 - ctrl + shift + esk -> task manager
// r1c0 - print('r1c0')
#include <Keyboard.h>

const int colPin1 = 2;    // First column pin
const int colPin2 = 8;    // Second column pin
const int rowPin1 = 5;    // First row pin
const int rowPin2 = 7;    // Second row pin

void setup() {
  pinMode(rowPin1, INPUT_PULLUP);
  pinMode(rowPin2, INPUT_PULLUP);
  pinMode(colPin1, OUTPUT);
  pinMode(colPin2, OUTPUT);

  // Set columns high initially
  digitalWrite(colPin1, HIGH);
  digitalWrite(colPin2, HIGH);

  Keyboard.begin();
}

void loop() {
  // Scan first column
  digitalWrite(colPin1, LOW);
  delayMicroseconds(10);

  if (digitalRead(rowPin1) == LOW) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('c');
    delay(100);
    Keyboard.releaseAll();
    unsigned long startTime = millis();
    while (digitalRead(rowPin1) == LOW) {
      if (millis() - startTime > 1000) break;
    }
    delay(50);
  }

  if (digitalRead(rowPin2) == LOW) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('q');
    delay(100);
    Keyboard.releaseAll();
    unsigned long startTime = millis();
    while (digitalRead(rowPin2) == LOW) {
      if (millis() - startTime > 1000) break;
    }
    delay(50);
  }

  digitalWrite(colPin1, HIGH);

  // Scan second column
  digitalWrite(colPin2, LOW);
  delayMicroseconds(10);

  if (digitalRead(rowPin1) == LOW) {
    Keyboard.print("r0c1");
    unsigned long startTime = millis();
    while (digitalRead(rowPin1) == LOW) {
      if (millis() - startTime > 1000) break;
    }
    delay(50);
  }

  if (digitalRead(rowPin2) == LOW) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_ESC);
    delay(100);
    Keyboard.releaseAll();
    unsigned long startTime = millis();
    while (digitalRead(rowPin2) == LOW) {
      if (millis() - startTime > 1000) break;
    }
    delay(50);
  }

  digitalWrite(colPin2, HIGH);

  delay(10);
}
