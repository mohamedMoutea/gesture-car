# Gesture-Controlled Robot Car

A computer vision and hardware control system that allows you to drive a robotic car using hand gestures detected via a webcam [cite: 1, 2, 3]. The project utilizes OpenCV and `cvzone` (a MediaPipe wrapper) to track hand landmark positions, map specific finger configurations to movement commands, and transmit final control signals over a serial connection (e.g., Bluetooth/USB) to an Arduino microcontroller driving an L298N motor driver module [cite: 1, 2, 3].

---

## Technical Insights & Design Philosophy

During early development, initial implementation attempts involved transmitting raw gesture landmark data directly to the Arduino, delegating the gesture parsing and decision-making logic to the microcontroller. However, these early algorithms consistently failed or experienced significant latency over time.

### The Breakthrough: Edge Offloading Strategy
Upon analyzing why previous iterations failed, a core architectural bottleneck became clear: **processing large streams of real-time spatial data is too computationally expensive for an 8-bit microcontroller (like the ATmega328P on the Arduino Uno).**

To resolve this, the processing pipeline was refactored based on a lightweight offloading principle:
* **Host (PC / Laptop):** Handles frame capture, deep-learning based hand landmark detection, spatial geometry calculations, and gesture decision logic [cite: 1, 2].
* **Communication Link:** Transmits only single ASCII character decision codes (`'F'`, `'B'`, `'L'`, `'R'`, `'S'`) across the serial pipeline [cite: 1, 3].
* **Target (Arduino):** Acts purely as an execution node—parsing incoming 1-byte command characters and asserting hardware pin states to control motor drivers instantly [cite: 3].

This shift eliminated serial transmission overhead and memory saturation on the Arduino, resulting in smooth, low-latency, real-time control [cite: 1, 3].

---

## Table of Contents
- [Features](#features)
- [System Architecture](#system-architecture)
- [Gesture Control Mapping](#gesture-control-mapping)
- [Hardware Requirements & Pinout](#hardware-requirements--pinout)
- [Software Requirements & Installation](#software-requirements--installation)
- [Circuit Setup](#circuit-setup)
- [Project Structure](#project-structure)
- [How to Run](#how-to-run)
- [Troubleshooting](#troubleshooting)

---

## Features
- **Real-Time Hand Tracking:** Detects hand landmarks at high frame rates using OpenCV and `cvzone.HandTrackingModule` [cite: 2].
- **Edge Gesture Offloading:** Evaluates finger geometry on the PC host to minimize serial payload size and microcontroller overhead [cite: 1, 2].
- **Serial Control Interface:** Transmits single-byte ASCII decision codes (`F`, `B`, `L`, `R`, `S`) via Python's `pyserial` interface [cite: 1].
- **Hardware Motor Control:** Arduino parses low-overhead decision bytes to control DC motors using an L298N dual H-bridge driver with PWM speed regulation [cite: 3].

---

## System Architecture

```
+-------------------+      +-----------------------+      +---------------------+      +---------------------+
|  Webcam Stream    | ---> | Host PC Processing    | ---> | Single-Byte Serial  | ---> | Arduino Execution   |
| (Video Frame Input)      | (cvzone Gesture Logic)       | Command Payload     |      | (L298N Motor Control)
+-------------------+      +-----------------------+      +---------------------+      +---------------------+
```

---

## Gesture Control Mapping

The host system evaluates the state of 5 fingers `[Thumb, Index, Middle, Ring, Pinky]`, where `1` indicates an extended finger and `0` indicates a folded finger [cite: 1, 2].

| Gesture (Fingers Up) | Finger Pattern `[T, I, M, R, P]` | Serial Signal Sent | Robot Action |
| :--- | :---: | :---: | :--- |
| **Index Finger Only** | `[0, 1, 0, 0, 0]` | `'F'` | **Forward** [cite: 1, 3] |
| **Index + Middle** | `[0, 1, 1, 0, 0]` | `'R'` | **Turn Right** [cite: 1, 3] |
| **4 Fingers Open** | `[0, 1, 1, 1, 1]` | `'L'` | **Turn Left** [cite: 1, 3] |
| **All 5 Fingers Open** | `[1, 1, 1, 1, 1]` | `'B'` | **Backward** [cite: 1, 3] |
| **Fist (0 Fingers)** | `[0, 0, 0, 0, 0]` | `'S'` | **Stop** [cite: 1, 3] |

---

## Hardware Requirements & Pinout

### Hardware Components
1. **Arduino Board** (e.g., Arduino Uno or Nano)
2. **L298N Dual H-Bridge Motor Driver** [cite: 3]
3. **2x DC Motors** with robot chassis and wheels [cite: 3]
4. **Bluetooth Module (HC-05 / HC-06)** or Direct USB Serial Link [cite: 1, 3]
5. **Power Source** (e.g., 2x 18650 Li-ion batteries for motors + battery holder)
6. **Webcam** (Built-in or USB camera) [cite: 2]
7. **Jumper Wires & Breadboard**

### Arduino Pin Connections

| Component Pin | Arduino Pin | Description |
| :--- | :--- | :--- |
| **HC-05 RX** | Pin 10 (`TX`) | Software Serial Transmitter [cite: 3] |
| **HC-05 TX** | Pin 11 (`RX`) | Software Serial Receiver [cite: 3] |
| **L298N ENA** | Pin 9 | Right Motor Enable (PWM Speed Control) [cite: 3] |
| **L298N ENB** | Pin 3 | Left Motor Enable (PWM Speed Control) [cite: 3] |
| **L298N IN1** | Pin 8 | Motor A Direction Control [cite: 3] |
| **L298N IN2** | Pin 7 | Motor A Direction Control [cite: 3] |
| **L298N IN3** | Pin 5 | Motor B Direction Control [cite: 3] |
| **L298N IN4** | Pin 4 | Motor B Direction Control [cite: 3] |

---

## Software Requirements & Installation

### 1. Python Environment Setup
Ensure Python 3.8+ is installed. Install the required dependencies [cite: 1, 2]:

```bash
pip install opencv-python cvzone pyserial
```

### 2. Arduino IDE Setup
- Download and install the [Arduino IDE](https://www.arduino.cc/en/software).
- Built-in `SoftwareSerial.h` library is required (included by default with the Arduino AVR core) [cite: 3].

---

## Circuit Setup

1. **Motor Driver Connection:**
   - Connect `ENA` and `ENB` on the L298N to PWM pins 9 and 3 on the Arduino [cite: 3].
   - Connect `IN1`, `IN2`, `IN3`, and `IN4` to Arduino pins 8, 7, 5, and 4 respectively [cite: 3].
   - Connect the DC motors to output terminals `OUT1/OUT2` and `OUT3/OUT4`.
   - Ensure a **common ground (GND)** connection between the Arduino and the L298N motor driver.

2. **Bluetooth / Serial Connection:**
   - Connect HC-05 `TX` to Arduino Pin 11 (`RX`) and HC-05 `RX` to Arduino Pin 10 (`TX`) [cite: 3].

---

## Project Structure

```
├── new.py             # Primary script for webcam capture and hand tracking display
├── controller.py      # Serial communication handler sending signals to Arduino
├── blu.ino            # Arduino sketch processing motor commands and pin outputs
└── README.md          # Project documentation
```

---

## How to Run

1. **Upload Code to Arduino:**
   - Connect your Arduino board via USB.
   - Open `blu.ino` in the Arduino IDE [cite: 3].
   - Select your target board and COM port, then click **Upload**.

2. **Configure Serial Port in Python:**
   - Open `controller.py` [cite: 1].
   - Update the COM port string (`"COM8"`) to match your active Bluetooth/USB COM port [cite: 1].

3. **Run the Main Application:**
   ```bash
   python new.py
   ```

4. **Operate the Car:**
   - Present hand gestures to the camera frame [cite: 2].
   - Press **`k`** while the video window is focused to safely exit [cite: 2].

---

## Troubleshooting

- **Serial Communication Error / Access Denied:**
  - Verify that the COM port configured in `controller.py` matches the active port in Device Manager [cite: 1].
  - Close the Arduino IDE Serial Monitor before running the Python script to prevent port locking.
- **Motors Rotating in Reverse:**
  - Swap the polarity of motor wires connected to the `OUT1/OUT2` or `OUT3/OUT4` terminals on the L298N driver, or adjust pin outputs in `blu.ino` [cite: 3].
- **Hand Detection Latency:**
  - Adjust room lighting or camera resolution in `new.py` for improved tracking responsiveness [cite: 2].
