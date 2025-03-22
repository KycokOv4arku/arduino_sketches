/*
 * rgb_led_controlled_by_encoder.ino
 * Arduino Pro Nano RGB LED Test with Encoder Control
 *
 * This sketch allows controlling an RGB LED with:
 * - Button press to cycle through 4 brightness levels
 *     100%, 75%, 50%, 0% (OFF).
 *     For my LED dimming lower than 50% turnes many colors off.
 * - Encoder rotation to cycle through fixed 8 colors (rainbow + white)
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
byte brightness = 0;                                // Index for brightness levels
const byte brightnessLevels[] = {255, 191, 128, 0}; // 100%, 75%, 50%, 0% (OFF)
const int numBrightnessLevels = 4;

// Fixed color array - RGB values in rainbow order starting with white
const byte colors[][3] = {
    {255, 255, 255}, // White
    {255, 0, 0},     // Red
    {255, 128, 0},   // Orange
    {255, 255, 0},   // Yellow
    {0, 255, 0},     // Green
    {0, 255, 255},   // Cyan
    {0, 0, 255},     // Blue
    {255, 0, 255}    // Purple
};
const int numColors = 8;

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
    // Encoder detection that changes color after two transitions on pin A
    currentEncoderA = digitalRead(ENCODER_S1_PIN);

    // Check for change in encoder A pin
    if (currentEncoderA != lastEncoderA)
    {
        static int transitionCount = 0;
        static int lastDirection = 0;
        static unsigned long lastChangeTime = 0;
        int direction = 0;

        // Simple debounce
        if (millis() - lastChangeTime > 5)
        {
            // Determine direction based on the state of B pin
            if (digitalRead(ENCODER_S2_PIN) != currentEncoderA)
            {
                // Clockwise
                direction = 1;
            }
            else
            {
                // Counterclockwise
                direction = -1;
            }

            // If direction changed, reset count
            if (lastDirection != 0 && direction != lastDirection)
            {
                transitionCount = 0;
            }

            // Count transitions in the same direction
            transitionCount++;
            lastDirection = direction;

            // Change color after two transitions in the same direction
            if (transitionCount >= 2)
            {
                if (direction == 1)
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
                transitionCount = 0; // Reset count after changing color
            }

            lastChangeTime = millis();
        }
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
        Serial.println("White");
        break;
    case 1:
        Serial.println("Red");
        break;
    case 2:
        Serial.println("Orange");
        break;
    case 3:
        Serial.println("Yellow");
        break;
    case 4:
        Serial.println("Green");
        break;
    case 5:
        Serial.println("Cyan");
        break;
    case 6:
        Serial.println("Blue");
        break;
    case 7:
        Serial.println("Purple");
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