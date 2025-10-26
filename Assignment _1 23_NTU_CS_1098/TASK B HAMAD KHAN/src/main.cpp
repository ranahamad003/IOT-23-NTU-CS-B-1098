// Task B 
// Name: Hamad Khan 
// Registration: 23_NTU_CS_1098 
// CS B 5th 

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Pin Definitions --- (ESP32 PINS)
#define BUTTON_PIN 14      // Single button for press detection
#define LED_PIN 5          // LED to toggle
#define BUZZER_PIN 25      // Buzzer for long press

// --- OLED Setup (I2C) ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- Press Detection Constants ---
const unsigned long LONG_PRESS_TIME = 1500; // 1.5 seconds for long press

// --- Program Variables ---
bool lastButtonState = HIGH;
bool buttonPressed = false;
unsigned long pressStartTime = 0;
bool ledState = false;

// --- Function Prototype --- (ADDED FOR VS CODE)
void updateDisplay();

void setup() {
  // Initialize Serial for debugging
  Serial.begin(115200);
  
  // Button Input with internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // LED Output
  pinMode(LED_PIN, OUTPUT);
  
  // Buzzer Output
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

  // Initial state
  digitalWrite(LED_PIN, LOW);
  updateDisplay();
  
  Serial.println("Task B - Press Detection System Started");
  Serial.println("Short press: Toggle LED");
  Serial.println("Long press (>1.5s): Activate buzzer");
}

void loop() {
  bool currentButtonState = digitalRead(BUTTON_PIN);
  
  // --- Button Press Detection ---
  
  // Button just pressed (HIGH to LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    pressStartTime = millis();
    buttonPressed = true;
    Serial.println("Button pressed - timing...");
  }
  
  // Button just released (LOW to HIGH)
  if (lastButtonState == LOW && currentButtonState == HIGH && buttonPressed) {
    buttonPressed = false;
    unsigned long pressDuration = millis() - pressStartTime;
    
    // --- Short Press Detection (< 1.5 seconds) ---
    if (pressDuration < LONG_PRESS_TIME) {
      // Toggle LED
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      Serial.print("SHORT PRESS - LED ");
      Serial.println(ledState ? "ON" : "OFF");
    }
  }
  
  // --- Long Press Detection (> 1.5 seconds) ---
  if (buttonPressed && (millis() - pressStartTime > LONG_PRESS_TIME)) {
    // Activate buzzer for long press
    tone(BUZZER_PIN, 1000, 500); // 1kHz tone for 500ms
    Serial.println("LONG PRESS - Buzzer activated");
    buttonPressed = false; // Prevent repeated triggers
  }
  
  lastButtonState = currentButtonState;
  delay(50); // Small delay for stability
}

/**
 * Update OLED Display - "Welcome" on top, "Hamad" on bottom
 */
void updateDisplay() {
  display.clearDisplay();
  
  // "Welcome" on top line (centered)
  display.setTextSize(2);
  display.setCursor(15, 15);
  display.println("Welcome");
  
  // "Hamad" on bottom line (centered)
  display.setTextSize(2);
  display.setCursor(35, 40);
  display.println("Hamad");
  
  display.display();
}