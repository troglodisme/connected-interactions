# 🧩 Connected Interactions Workshop

**Techniques for prototyping hardware / app interactions**  
Led by Giulio Ammendola · April 2025

This hands-on workshop explores how to quickly spin up connected prototypes where hardware talks to software — whether via Wi-Fi, Bluetooth, or the cloud.

We’ll walk through **three key techniques** for creating responsive, interactive systems using ESP32-based devices.

---

## 💡 What You'll Learn

- Serve web interfaces directly from your device
- Communicate with apps using Bluetooth Low Energy (BLE)
- Send and receive data through Firebase
- Combine hardware, code, and design into fast functional prototypes

---

## 📦 Project Examples

Each example includes:
- ✅ Working code
- ✅ Easy-to-follow comments
- ✅ Room for creative extensions

### 1. `esp_web_server/`
**ESP32 Access Point + Web UI**  
The ESP32 creates its own Wi-Fi network and hosts a sleek control dashboard in your browser. Includes:
- Live temperature readout
- Buttons to control a **servo** and the **onboard LED**

### 2. `esp_ble_peripheral/`
**ESP32 as BLE Device**  
The ESP32 acts as a Bluetooth peripheral:
- Streams a simulated temperature value via notifications
- Accepts a write command to toggle the onboard LED  
Can be tested with:
- 📱 [nRF Connect](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-mobile) (recommended)
- 🧑‍💻 Optional SwiftUI BLE app template

### 3. `esp_firebase_comms/`
**ESP32 Sends Data to Firebase**  
The ESP32 connects to Wi-Fi and pushes sensor data to [Firebase Realtime Database](https://firebase.google.com/products/realtime-database), where it can be visualized or accessed by multiple devices. Includes:
- Basic Firebase setup for storing temperature data
- Template for reading the data in a browser or mobile app

Great for:
- Storing time-series or live data
- Multi-device syncing
- Creating cloud-connected dashboards

---

## 🛠 What You Need

- A Seeed XIAO ESP32C3 (or similar ESP32 board)
- USB cable + Arduino IDE
- (Optional) DHT11/DHT22 sensor, servo motor, external LED
- (For BLE) Xcode or nRF Connect mobile app
- (For Firebase) A free Firebase account + Realtime Database setup

---

## 🧪 Stretch Exercises

Each example includes bonus ideas to keep building:
- Add new control elements
- Display charts with Chart.js
- Read real sensors (e.g. IMU, air quality)
- Build a multi-device dashboard

---

## 📂 Folder Overview

```bash
connected-interactions/
├── esp_web_server/         # Access Point + Dashboard + Servo/LED
├── esp_ble_peripheral/     # BLE Peripheral with temp + LED control
├── esp_firebase_comms/     # Firebase Realtime DB communication
└── README.md               # You're here!