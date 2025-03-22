/*
 * Arduino Pro Nano RGB LED Test
 * 
 * This sketch cycles an RGB LED through different colors
 * with a 1 second delay between changes.
 * 
 * Pin Connections:
 * D12 - Red
 * D11 - Green
 * D10 - Blue
 * D9 - GND (set as OUTPUT and LOW)
 */

// Define pins
const int RED_PIN = 12;
const int GREEN_PIN = 11;
const int BLUE_PIN = 10;
const int GND_PIN = 9;

// Delay between color changes (in milliseconds)
const int DELAY_TIME = 1000;

void setup() {
  // Initialize all pins as outputs

}

void loop() {
  // Red
  setColor(HIGH, LOW, LOW);
  Serial.println("RED");
  delay(DELAY_TIME);
  
  // Green
  setColor(LOW, HIGH, LOW);
  Serial.println("GREEN");
  delay(DELAY_TIME);
  
  // Blue
  setColor(LOW, LOW, HIGH);
  Serial.println("BLUE");
  delay(DELAY_TIME);
  
  // Yellow (Red + Green)
  setColor(HIGH, HIGH, LOW);
  Serial.println("YELLOW");
  delay(DELAY_TIME);
  
  // Purple (Red + Blue)
  setColor(HIGH, LOW, HIGH);
  Serial.println("PURPLE");
  delay(DELAY_TIME);
  
  // Cyan (Green + Blue)
  setColor(LOW, HIGH, HIGH);
  Serial.println("CYAN");
  delay(DELAY_TIME);
  
  // White (All colors)
  setColor(HIGH, HIGH, HIGH);
  Serial.println("WHITE");
  delay(DELAY_TIME);
}

// Function to set RGB color
void setColor(bool red, bool green, bool blue) {
  digitalWrite(RED_PIN, red);
  digitalWrite(GREEN_PIN, green);
  digitalWrite(BLUE_PIN, blue);
}