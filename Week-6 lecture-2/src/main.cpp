#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- Pin configuration ---
#define DHTPIN 14        // DHT22 data pin
#define DHTTYPE DHT22    // Change to DHT11 if using DHT11
#define LDR_PIN 34       // LDR analog pin

#define SDA_PIN 21       // I2C SDA pin
#define SCL_PIN 22       // I2C SCL pin

// --- OLED setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- DHT sensor setup ---
DHT dht(DHTPIN, DHTTYPE);

// --- Setup function ---
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Sensor System Starting...");

  // Initialize I2C on custom pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed!");
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Initializing Sensors...");
  display.display();

  // Initialize DHT sensor
  dht.begin();
  delay(1000);
}

// --- Main loop ---
void loop() {
  // --- Read Temperature & Humidity ---
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // --- Read LDR value ---
  int adcValue = analogRead(LDR_PIN);
  float voltage = (adcValue / 4095.0) * 3.3;

  // --- Check if DHT read failed ---
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading DHT22 sensor!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Sensor Error!");
    display.display();
    delay(2000);
    return;
  }

  // --- Print readings on Serial Monitor ---
  Serial.println("==================================");
  Serial.printf("Temperature: %.2f °C\n", temperature);
  Serial.printf("Humidity: %.2f %%\n", humidity);
  Serial.printf("LDR ADC: %d  |  Voltage: %.2f V\n", adcValue, voltage);
  Serial.println("==================================\n");

  // --- Display on OLED ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("🌡 DHT22 + LDR Monitor");
  
  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 28);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0, 42);
  display.print("LDR: ");
  display.print(adcValue);
  display.setCursor(0, 54);
  display.print("Volt: ");
  display.print(voltage, 2);
  display.println(" V");

  display.display();

  delay(2000); // Update every 2 seconds
}