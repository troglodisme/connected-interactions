#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Adafruit_NeoPixel.h>

// Firebase helper includes
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// ==== Wi-Fi & Firebase Config ====
#define WIFI_SSID "YOUR-SSID"
#define WIFI_PASSWORD "YOUR-PASSWORD"
#define API_KEY "YOURKEY"
#define DATABASE_URL "YOURDATABASE"

// ==== Firebase Objects ====
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ==== NeoPixel Setup ====
#define LED_PIN 23
#define NUM_PIXELS 1
Adafruit_NeoPixel pixels(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ==== Shared State ====
String firebasePath = "/workshop_data/group1";
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);

  // Setup NeoPixel
  pixels.begin();
  pixels.clear();
  pixels.show();

  // Wi-Fi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\n✅ Wi-Fi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("✅ Firebase signup OK");
    signupOK = true;
  } else {
    Serial.printf("❌ Firebase signup failed: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // --- Push fake temperature ---
    float temp = 20.0 + random(-300, 300) / 10.0;
    String tempPath = firebasePath + "/temperature";
    if (Firebase.RTDB.setFloat(&fbdo, tempPath.c_str(), temp)) {
      Serial.printf("✅ Temp sent: %.2f°C to %s\n", temp, tempPath.c_str());
    } else {
      Serial.printf("❌ Temp send failed: %s\n", fbdo.errorReason().c_str());
    }

    // --- Read LED state and update NeoPixel ---
    String ledPath = firebasePath + "/led";
    if (Firebase.RTDB.getBool(&fbdo, ledPath.c_str())) {
      bool ledState = fbdo.boolData();

      if (ledState) {
        pixels.setPixelColor(0, pixels.Color(0, 255, 0));  // Green ON
        Serial.println("✅ LED ON (green)");
      } else {
        pixels.clear();  // OFF
        Serial.println("✅ LED OFF");
      }
      pixels.show();
    } else {
      Serial.printf("⚠️ LED path missing, initializing as false...\n");
      if (Firebase.RTDB.setBool(&fbdo, ledPath.c_str(), false)) {
        Serial.println("✔️ LED path created");
      } else {
        Serial.printf("❌ Failed to set LED path: %s\n", fbdo.errorReason().c_str());
      }
    }
  }
}