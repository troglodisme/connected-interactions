#include <Arduino.h>
#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>

// === Pin Definitions ===
#define LED_PIN 23         // RGB NeoPixel
#define SIMPLE_LED_PIN 10  // Simple on/off LED
#define NUM_PIXELS 1

// === BLE Services and Characteristics ===
BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214");

// RGB LED control (3 bytes: R, G, B)
BLECharacteristic ledColorChar("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite, 3);

// Simple LED control (1 byte: 0 = OFF, 1 = ON)
BLEByteCharacteristic simpleLedChar("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

// Temperature mock service (optional)
BLEService envService("12345678-1234-5678-1234-56789abcdef0");
BLEFloatCharacteristic tempChar("12345678-1234-5678-1234-56789abcdea2", BLERead | BLENotify);

// === LED Setup ===
Adafruit_NeoPixel pixels(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// === Timing ===
unsigned long lastUpdate = 0;
const int UPDATE_INTERVAL = 1000; // ms

void setup() {
  Serial.begin(115200);

  // LED Pins
  pinMode(SIMPLE_LED_PIN, OUTPUT);
  digitalWrite(SIMPLE_LED_PIN, LOW);

  pixels.begin();
  pixels.clear();
  pixels.show();

  // BLE Init
  if (!BLE.begin()) {
    Serial.println("BLE init failed");
    while (1);
  }

  BLE.setLocalName("BLE-Device");
  BLE.setDeviceName("BLE-Device");

  // BLE Service Setup
  ledService.addCharacteristic(ledColorChar);
  ledService.addCharacteristic(simpleLedChar);
  BLE.addService(ledService);

  ledColorChar.writeValue((const uint8_t[]){0, 0, 0}, 3); // Start with RGB off
  simpleLedChar.writeValue(0);                            // Simple LED OFF

  envService.addCharacteristic(tempChar);
  BLE.addService(envService);
  tempChar.writeValue(22.5);

  BLE.advertise();
  Serial.println("BLE is now advertising...");
}

void loop() {
  BLE.poll();

  if (millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();

    // === Mock Temp Update ===
    float temp = 22.5 + random(-5, 5) * 0.1;
    tempChar.writeValue(temp);
    Serial.printf("Temperature: %.1f°C\n", temp);

    // === RGB LED Update ===
    if (ledColorChar.written()) {
      uint8_t color[3];
      ledColorChar.readValue(color, 3);
      pixels.setPixelColor(0, pixels.Color(color[0], color[1], color[2]));
      pixels.show();
      Serial.printf("RGB LED -> R:%d G:%d B:%d\n", color[0], color[1], color[2]);
    }

    // === Simple LED Update ===
    if (simpleLedChar.written()) {
      uint8_t simpleValue = simpleLedChar.value();
      digitalWrite(SIMPLE_LED_PIN, simpleValue ? HIGH : LOW);
      Serial.printf("Simple LED -> %s\n", simpleValue ? "ON" : "OFF");
    }
  }
}