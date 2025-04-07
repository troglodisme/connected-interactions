#include <WiFi.h>          // For setting up WiFi Access Point
#include <WebServer.h>     // Simple web server for handling HTTP routes

#define LED_PIN 10         // Pin where normal LED is connected

WebServer server(80);      // Web server on port 80

const char* ssid = "ChangeThisWifiName";
const char* password = "password";

bool ledOn = false;
float fakeTemp = 22.0;
bool increasing = true;

// ==================== HTML GENERATOR ====================
String getStyledHTML(float temp) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Local Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: 'Courier New', Courier, monospace;
      background: #e0e0e0;
      color: #111;
      margin: 0;
      padding: 0;
    }
    .header {
      background-color: #111;
      color: white;
      padding: 18px;
      text-align: center;
      font-size: 1.2rem;
      font-weight: bold;
    }
    .container {
      width: 90%;
      max-width: 400px;
      margin: 20px auto;
      background: #f9f9f9;
      padding: 20px;
      box-shadow: 4px 4px 0 #999;
      border: 2px solid #111;
    }
    .data-block {
      border-left: 4px solid #444;
      padding-left: 10px;
      margin-bottom: 16px;
    }
    .label {
      font-weight: bold;
      font-size: 0.9rem;
      text-transform: uppercase;
      color: #111;
    }
    .value {
      font-size: 1.4rem;
      font-weight: bold;
      margin: 4px 0;
      color: #111;
    }
    .desc {
      font-size: 0.8rem;
      color: #555;
      font-style: italic;
    }
    .button {
      display: block;
      width: 100%;
      margin-top: 20px;
      background: #111;
      color: white;
      padding: 12px;
      font-size: 1rem;
      font-weight: bold;
      border: none;
      cursor: pointer;
    }
    .button:hover {
      background: #333;
    }
    .footer {
      text-align: center;
      font-size: 0.75rem;
      color: #333;
      margin-top: 24px;
      font-weight: bold;
    }
    a {
      color: #111;
      text-decoration: none;
    }
    a:hover {
      text-decoration: underline;
    }
  </style>
</head>
<body>
  <div class="header">Simple Monitor</div>
  <div class="container">
    <div class="data-block">
      <div class="label">Temperature</div>
      <div class="value">)rawliteral" + String(temp, 1) + R"rawliteral( °C</div>
      <div class="desc">Fake simulated value</div>
    </div>

    <form action="/toggle" method="POST">
      <button class="button" type="submit">Turn LED )rawliteral" + String(ledOn ? "Off" : "On") + R"rawliteral(</button>
    </form>

    <div class="footer">
      <p>giulio ammendola, 2025</p>
    </div>
  </div>
</body>
</html>
)rawliteral";

  return html;
}

// ==================== HTTP ROUTES ====================
void handleRoot() {
  server.send(200, "text/html", getStyledHTML(fakeTemp));
}

void handleToggle() {
  ledOn = !ledOn;

  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);

  Serial.println(ledOn ? "LED is ON" : "LED is OFF");

  server.sendHeader("Location", "/", true);
  server.send(303);
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Start with LED off

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.on("/toggle", HTTP_POST, handleToggle);
  server.begin();

  Serial.println("Web server started.");
}

// ==================== LOOP ====================
void loop() {
  server.handleClient();

  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 3000) {
    if (increasing) {
      fakeTemp += 0.1;
      if (fakeTemp > 24.0) increasing = false;
    } else {
      fakeTemp -= 0.1;
      if (fakeTemp < 22.0) increasing = true;
    }
    lastUpdate = millis();
  }
}