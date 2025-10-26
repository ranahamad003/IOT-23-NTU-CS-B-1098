// Task A 
// Name: Hamad Khan
// Registration: 23_NTU_CS_1098
// CS B 5th

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Pin Definitions --- (ESP32 PINS)
#define BUTTON_MODE_PIN 14
#define BUTTON_RESET_PIN 27
#define LED1_PIN 5
#define LED2_PIN 18
#define LED3_PIN 19    // PWM capable pin
#define BUZZER_PIN 25

// --- OLED Setup (I2C) ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Program Variables ---
int currentMode = 0;
const int TOTAL_MODES = 4;
unsigned long previousBlinkTime = 0;
int blinkState = 0; // 0=LED1, 1=LED2, 2=LED3
int fadeValue = 0;
bool fadeDirection = true;

// --- Mode Names for Display ---
const char* modeNames[] = {
  "ALL LEDs OFF",
  "ONE-BY-ONE Blink",
  "ALL LEDs ON",
  "PWM Fade"
};

// --- Function Prototypes ---
void testAllLEDs();
void resetLEDs();
void updateDisplay();
void oneByOneBlink();
void pwmFade();

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  
  // Button Inputs with internal pull-up resistors
  pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RESET_PIN, INPUT_PULLUP);

  // LED Outputs
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // --- OLED Display Initialization ---
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 allocation failed!");
  } else {
    Serial.println("OLED initialized successfully!");
  }

  // Display setup
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Test all LEDs at startup
  testAllLEDs();

  resetLEDs();
  updateDisplay();
  
  Serial.println(" Task 1 - LED Mode Cycling System Started");
  Serial.println("Press Button 1 (Pin 14) to change modes");
  Serial.println("Press Button 2 (Pin 27) to reset to OFF");
}

void loop() {
  // --- Mode Button: Cycle through modes ---
  if (digitalRead(BUTTON_MODE_PIN) == LOW) {
    currentMode = (currentMode + 1) % TOTAL_MODES;
    resetLEDs();
    updateDisplay();
    Serial.print(" Mode changed to: ");
    Serial.println(modeNames[currentMode]);
    delay(300);
  }

  // --- Reset Button: Return to OFF mode ---
  if (digitalRead(BUTTON_RESET_PIN) == LOW) {
    currentMode = 0;
    resetLEDs();
    updateDisplay();
    Serial.println(" Reset to OFF mode");
    delay(300);
  }

  // --- Execute Current Mode Behavior ---
  switch(currentMode) {
    case 0: // Mode 0: All LEDs OFF
      resetLEDs();
      break;
      
    case 1: // Mode 1: One-by-One Blinking
      oneByOneBlink();
      break;
      
    case 2: // Mode 2: All LEDs ON
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, HIGH);
      digitalWrite(LED3_PIN, HIGH);
      break;
      
    case 3: // Mode 3: PWM Fade Effect
      pwmFade();
      break;
  }
}

/**
 * Test all LEDs at startup
 */
void testAllLEDs() {
  Serial.println(" Testing LED1 (Pin 5)...");
  digitalWrite(LED1_PIN, HIGH);
  delay(500);
  digitalWrite(LED1_PIN, LOW);
  delay(200);
  
  Serial.println(" Testing LED2 (Pin 18)...");
  digitalWrite(LED2_PIN, HIGH);
  delay(500);
  digitalWrite(LED2_PIN, LOW);
  delay(200);
  
  Serial.println(" Testing LED3 (Pin 19)...");
  digitalWrite(LED3_PIN, HIGH);
  delay(500);
  digitalWrite(LED3_PIN, LOW);
  delay(200);
  
  Serial.println(" All LEDs tested successfully!");
}

/**
 * Reset all LEDs to OFF state
 */
void resetLEDs() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  analogWrite(LED3_PIN, 0); // Turn off PWM
  blinkState = 0;
}

/**
 * One-by-One LED Blinking Pattern
 * LEDs light up sequentially: LED1 → LED2 → LED3 → repeat
 */
void oneByOneBlink() {
  unsigned long currentTime = millis();
  if (currentTime - previousBlinkTime >= 500) {
    previousBlinkTime = currentTime;
    
    // Turn off all LEDs first
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    
    // Turn on one LED at a time in sequence
    switch(blinkState) {
      case 0:
        digitalWrite(LED1_PIN, HIGH);
        Serial.println(" LED1 ON (Pin 5)");
        break;
      case 1:
        digitalWrite(LED2_PIN, HIGH);
        Serial.println(" LED2 ON (Pin 18)");
        break;
      case 2:
        digitalWrite(LED3_PIN, HIGH);
        Serial.println(" LED3 ON (Pin 19)");
        break;
    }
    
    // Move to next LED in sequence
    blinkState = (blinkState + 1) % 3;
  }
}

/**
 * PWM Fade Effect on LED3 with LED1 & LED2 following brightness
 * Using analogWrite() which works in Wokwi ESP32
 */
void pwmFade() {
  static unsigned long lastFadeTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastFadeTime >= 30) {
    lastFadeTime = currentTime;

    fadeValue += (fadeDirection ? 5 : -5);
    
    if (fadeValue >= 255) {
      fadeValue = 255;
      fadeDirection = false;
    } else if (fadeValue <= 0) {
      fadeValue = 0;
      fadeDirection = true;
    }

    // Use analogWrite for PWM in Wokwi ESP32
    analogWrite(LED3_PIN, fadeValue);
    
    // LED1 and LED2 follow brightness
    digitalWrite(LED1_PIN, fadeValue > 100);
    digitalWrite(LED2_PIN, fadeValue > 100);
  }
}

/**
 * Update OLED Display with Current System State
 */
void updateDisplay() {
  display.clearDisplay();
  
  display.setTextSize(2);
  display.setCursor(5, 25);
  display.println("Welcome Hamad");
  
  display.display();
}