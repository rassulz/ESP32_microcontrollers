# ESP32 Microcontrollers — Projects & Experiments ⚡🤖

This repository is my collection of **ESP32-based projects** and reusable experiments (motors,servos,web control,joystick/buttons UI,ESP32-CAM ideas,etc.).It documents my hands-on work with ESP32 + different devices for different mini-projects.

---

## 🎥 Demo — ESP32 Projects (ITFest 2024)

Click on the image below to watch the demo recorded during ITFest 2024:

[![ESP32 Demo](https://img.youtube.com/vi/HK_XoP7gYFU/0.jpg)](https://youtube.com/shorts/HK_XoP7gYFU)

---

## ⭐ Main Project

### `ESP32_CAR_SERVER_JOISTIK/ESP32_CAR_SERVER_JOISTIK.ino`

This is the **main and most complete code** in the repository.

**What it is:**  
An ESP32-based **Wi-Fi car control server** where the car is controlled from a browser UI using a **joystick-style controller**.

**What I focused on in this code:**
- building a stable control loop for a small ESP32 car
- mapping joystick input → motor commands (direction + speed control)
- making controls usable from real devices (phone/laptop)
- keeping the project simple enough for students to understand and modify

---

## 🧩 What Else Is Inside (repo folders)

This repo contains multiple sub-projects and prototypes created during development and testing:

- `ESP32_CAR_SERVER_BOTTONS` / `..._phone` / `..._laptop` — different **button-based** control interfaces (adapted for different screens)
- `CAR_ESP32_SERVO` + `servo_manipulating` — experiments with **servo control** (movement logic,positioning)
- `Stepper_motor` — stepper motor control experiments
- `blinking_led` — basic ESP32 bring-up / GPIO tests
- `joystick_control` + `joinstik_simply` — joystick control prototypes (simplified versions / quick tests)
- `Camera_Car__1_` + `car_web_double` — experiments around “car + web + camera” style setups
- `libraries` — helper code used across projects

These folders show my iteration process:from basic hardware tests → control prototypes → a more complete “car server” project.

---

## 🏆 ITFest 2024 (Mentor Project)

This repository includes code and experiments created for **ITFest 2024**,where I worked as a **mentor in the Robotics sector** and helped student teams solve technical problems and improve their robot designs.On the second day I also supported the robotics soccer competition as an **assistant referee**.

---

## 🛠 Tech Stack

- **ESP32**
- **Arduino / C++ (.ino)**
- **Wi-Fi control (browser-based UI)**
- Motors / servos / stepper experiments
- Simple modular prototypes for fast testing

---

## 🚀 How to Run the Main Code (Quick Start)

1. Open:
in Arduino IDE (or PlatformIO if you prefer).
2. Install the ESP32 board package in Arduino IDE.
3. Select your ESP32 board + COM port.
4. Upload the sketch to ESP32.
5. Connect to the ESP32 network (or the configured Wi-Fi mode,depending on your setup).
6. Open the control page in your browser and drive the car using the joystick UI.

> Note:Exact wiring depends on your motor driver + car chassis setup (the code in this repo reflects my build and experiments).

---

## 👤 Author

**Rassul Zeynulla**  
- LinkedIn: https://www.linkedin.com/in/rassul-zeynulla/  
- ITFest post: https://www.linkedin.com/posts/rassul-zeynulla_itfest-2024-diving-into-the-world-of-activity-7282752582987464706--vkf
