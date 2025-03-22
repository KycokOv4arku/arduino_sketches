/*
 * Arduino Pro Nano RGB LED Test with Encoder Control
 *
 * This sketch allows controlling an RGB LED with:
 * - Button press to cycle through 5 brightness levels
 * - Encoder rotation to cycle through fixed colors
 *
 * RGB LED Pin Connections:
 * D12 - Red
 * D11 - Green
 * D10 - Blue
 * D9 - GND (set as OUTPUT and LOW)
 *
 * Encoder Pin Connections:
 * D6 - GND
 * D5 - S1
 * D4 - S2
 * D3 - Button
 * D2 - 5V (set as OUTPUT and HIGH)
 */

// Define RGB LED pins
const int RED_PIN = 12;
const int GREEN_PIN = 11;
const int BLUE_PIN = 10;
const int LED_GND_PIN = 9;

// Define encoder pins
const int ENCODER_GND_PIN = 6;
const int ENCODER_S1_PIN = 5;
const int ENCODER_S2_PIN = 4;
const int ENCODER_BTN_PIN = 3;
const int ENCODER_VCC_PIN = 2;

// Variables for color and brightness control
int lastEncoderA = LOW;
int currentEncoderA;
int colorIndex = 0;

// Brightness control
byte brightness = 0;                                    // Index for brightness levels
const byte brightnessLevels[] = {255, 191, 128, 64, 0}; // 100%, 75%, 50%, 25%, 0% (OFF)
const int numBrightnessLevels = 5;

// Fixed color array - RGB values for each color
const byte colors[][3] = {
    {255, 0, 0},     // Red
    {0, 255, 0},     // Green
    {0, 0, 255},     // Blue
    {255, 255, 0},   // Yellow
    {255, 0, 255},   // Purple
    {0, 255, 255},   // Cyan
    {255, 255, 255}, // White
    {255, 128, 0},   // Orange
    {128, 0, 128}    // Dark Purple
};
const int numColors = 9;

// Variables for button debouncing
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200; // Milliseconds
bool lastButtonState = HIGH;

void setup()
{
    // Initialize RGB LED pins as outputs
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    pinMode(LED_GND_PIN, OUTPUT);

    // Set LED GND pin LOW
    digitalWrite(LED_GND_PIN, LOW);

    // Initialize encoder pins
    pinMode(ENCODER_GND_PIN, OUTPUT);
    pinMode(ENCODER_S1_PIN, INPUT);
    pinMode(ENCODER_S2_PIN, INPUT);
    pinMode(ENCODER_BTN_PIN, INPUT_PULLUP); // Use internal pull-up resistor
    pinMode(ENCODER_VCC_PIN, OUTPUT);

    // Set encoder power pins
    digitalWrite(ENCODER_GND_PIN, LOW);
    digitalWrite(ENCODER_VCC_PIN, HIGH);

    // Initialize serial communication for debugging
    Serial.begin(9600);
    Serial.println("Arduino Pro Nano RGB LED with Encoder Test");
    Serial.println("Press button to cycle brightness levels");
    Serial.println("Rotate encoder to change colors");

    // Initialize encoder state
    lastEncoderA = digitalRead(ENCODER_S1_PIN);

    // Start with full brightness and first color
    updateRGB();
    printCurrentColor();
}

void loop()
{
    // Check for encoder rotation (simple state-based detection)
    currentEncoderA = digitalRead(ENCODER_S1_PIN);

    // If we detected a transition on encoder A pin
    if (currentEncoderA != lastEncoderA)
    {
        // If encoder B pin is different than A pin, we're rotating clockwise
        // Otherwise we're rotating counterclockwise
        if (digitalRead(ENCODER_S2_PIN) != currentEncoderA)
        {
            // Clockwise - increment color
            colorIndex = (colorIndex + 1) % numColors;
        }
        else
        {
            // Counterclockwise - decrement color
            colorIndex = (colorIndex + numColors - 1) % numColors;
        }

        updateRGB();
        printCurrentColor();

        // Short delay to prevent multiple readings
        delay(10);
    }

    lastEncoderA = currentEncoderA;

    // Check for button press
    bool reading = digitalRead(ENCODER_BTN_PIN);

    // If button is pressed (LOW) and enough time has passed for debounce
    if (reading == LOW && (millis() - lastButtonPress > debounceDelay))
    {
        if (lastButtonState == HIGH)
        { // Only trigger once on button press
            lastButtonPress = millis();

            // Cycle through brightness levels
            brightness = (brightness + 1) % numBrightnessLevels;

            Serial.print("Brightness level: ");
            switch (brightness)
            {
            case 0:
                Serial.println("100%");
                break;
            case 1:
                Serial.println("75%");
                break;
            case 2:
                Serial.println("50%");
                break;
            case 3:
                Serial.println("25%");
                break;
            case 4:
                Serial.println("OFF");
                break;
            }

            updateRGB();
        }
    }

    lastButtonState = reading;

    // Small delay to limit CPU usage
    delay(5);
}

// Function to print current color to Serial
void printCurrentColor()
{
    Serial.print("Current color: ");
    switch (colorIndex)
    {
    case 0:
        Serial.println("Red");
        break;
    case 1:
        Serial.println("Green");
        break;
    case 2:
        Serial.println("Blue");
        break;
    case 3:
        Serial.println("Yellow");
        break;
    case 4:
        Serial.println("Purple");
        break;
    case 5:
        Serial.println("Cyan");
        break;
    case 6:
        Serial.println("White");
        break;
    case 7:
        Serial.println("Orange");
        break;
    case 8:
        Serial.println("Dark Purple");
        break;
    }
}

// Function to update RGB LED with current color and brightness
void updateRGB()
{
    // Get current color
    byte r = colors[colorIndex][0];
    byte g = colors[colorIndex][1];
    byte b = colors[colorIndex][2];

    // Apply brightness level
    byte brightnessFactor = brightnessLevels[brightness];

    // Calculate dimmed RGB values
    byte dimmedR = (r * brightnessFactor) / 255;
    byte dimmedG = (g * brightnessFactor) / 255;
    byte dimmedB = (b * brightnessFactor) / 255;

    // Set the LED colors
    analogWrite(RED_PIN, dimmedR);
    analogWrite(GREEN_PIN, dimmedG);
    analogWrite(BLUE_PIN, dimmedB);
}