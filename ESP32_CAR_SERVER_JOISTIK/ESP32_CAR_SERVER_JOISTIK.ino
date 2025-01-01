#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid     = "Borealis Laboratory";
const char* password = "artofwar3";

// Create an instance of the WebServer on port 80
WebServer server(80);

// Motor 1
int motor1Pin1 = 18;  // Changed pin
int motor1Pin2 = 19;  // Changed pin
int enable1Pin = 23;  // Changed pin (PWM pin for Motor 1)

// Motor 2
int motor2Pin1 = 32;  // Changed pin
int motor2Pin2 = 33;  // Changed pin
int enable2Pin = 25;  // Changed pin (PWM pin for Motor 2)

// Setting PWM properties
int dutyCycle = 0; // Duty cycle for motors

String valueString = String(0);

void handleRoot() {
  const char html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
      html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }
      #joystick { 
        width: 200px; 
        height: 200px; 
        background-color: #4CAF50; 
        border-radius: 50%; 
        margin: 40px auto; 
        position: relative; 
        touch-action: none; 
        border: 4px solid #2c6c2f;
      }
      #knob { 
        width: 80px; 
        height: 80px; 
        background-color: #555555; 
        border-radius: 50%; 
        position: absolute; 
        top: 60px; 
        left: 60px; 
        transition: all 0.1s ease-in-out;
      }
      #motorSpeedDisplay { 
        margin-top: 20px; 
        font-size: 18px; 
      }
      #motorSlider {
        width: 80%;
        margin-top: 20px;
      }
      body {
        background-color: #f0f0f0;
        padding: 20px;
      }
    </style>
    <script>
      let joystickCenter = { x: 100, y: 100 };
      let isDragging = false;
      let lastDirection = "";

      document.addEventListener("DOMContentLoaded", () => {
        const joystick = document.getElementById("joystick");
        const knob = document.getElementById("knob");

        joystick.addEventListener("pointerdown", (event) => {
          isDragging = true;
          moveKnob(event);
        });

        document.addEventListener("pointermove", (event) => {
          if (isDragging) {
            moveKnob(event);
          }
        });

        document.addEventListener("pointerup", () => {
          isDragging = false;
          resetKnob();
          stopRobot();
        });
      });

      function moveKnob(event) {
        const joystick = document.getElementById("joystick");
        const knob = document.getElementById("knob");
        const rect = joystick.getBoundingClientRect();
        const x = event.clientX - rect.left - joystickCenter.x;
        const y = event.clientY - rect.top - joystickCenter.y;

        const angle = Math.atan2(y, x);
        const distance = Math.min(Math.sqrt(x * x + y * y), 80); // Увеличил радиус до 80px
        knob.style.left = `${100 + distance * Math.cos(angle) - 40}px`;
        knob.style.top = `${100 + distance * Math.sin(angle) - 40}px`;

        if (distance > 20) {
          const direction = getDirection(angle); // Вызов функции getDirection
          if (direction !== lastDirection) {
            lastDirection = direction;
            fetch(`/${direction}`);
          }
        }
      }

      function getDirection(angle) {
        if (angle >= -Math.PI / 8 && angle < Math.PI / 8) return "right";
        if (angle >= Math.PI / 8 && angle < 3 * Math.PI / 8) return "right-up";
        if (angle >= 3 * Math.PI / 8 && angle < 5 * Math.PI / 8) return "forward";
        if (angle >= 5 * Math.PI / 8 && angle < 7 * Math.PI / 8) return "left-up";
        if (angle >= 7 * Math.PI / 8 || angle < -7 * Math.PI / 8) return "left";
        if (angle >= -7 * Math.PI / 8 && angle < -5 * Math.PI / 8) return "left-down";
        if (angle >= -5 * Math.PI / 8 && angle < -3 * Math.PI / 8) return "reverse";
        if (angle >= -3 * Math.PI / 8 && angle < -Math.PI / 8) return "right-down";
        return "stop";
      }

      function resetKnob() {
        const knob = document.getElementById("knob");
        knob.style.left = "60px";
        knob.style.top = "60px";
        lastDirection = ""; // Сбрасываем последнее направление
      }

      function stopRobot() {
        fetch('/stop'); // Отправляем команду остановки робота
      }

      function updateMotorSpeed(pos) {
        document.getElementById("motorSpeed").innerHTML = pos;
        fetch(`/speed?value=${pos}`);
      }
    </script>
  </head>
  <body>
    <h1>ESP32 Motor Control with Joystick</h1>
    <div id="joystick">
      <div id="knob"></div>
    </div>
    <p id="motorSpeedDisplay">Motor Speed: <span id="motorSpeed">0</span></p>
    <input type="range" min="0" max="100" step="25" id="motorSlider" oninput="updateMotorSpeed(this.value)" value="0"/>
  </body>
  </html>)rawliteral";
  server.send(200, "text/html", html);
}

void handleLeftUp() {
  Serial.println("Right-Up");
  analogWrite(enable1Pin, 200); // Левый двигатель быстрее
  analogWrite(enable2Pin, 100); // Правый двигатель медленнее
  digitalWrite(motor1Pin1, LOW); 
  digitalWrite(motor1Pin2, HIGH); // Левый двигатель вперед
  digitalWrite(motor2Pin1, LOW); 
  digitalWrite(motor2Pin2, HIGH); // Правый двигатель вперед
  server.send(200);
}
void handleRightDown() {
  Serial.println("Right-Down");
  analogWrite(enable1Pin, 100); // Левый двигатель медленнее
  analogWrite(enable2Pin, 200); // Правый двигатель быстрее
  digitalWrite(motor1Pin1, HIGH); 
  digitalWrite(motor1Pin2, LOW); // Левый двигатель назад
  digitalWrite(motor2Pin1, HIGH); 
  digitalWrite(motor2Pin2, LOW); // Правый двигатель назад
  server.send(200);
}
void handleRightUp() {
  Serial.println("Left-Up");
  analogWrite(enable1Pin, 100); // Левый двигатель медленнее
  analogWrite(enable2Pin, 200); // Правый двигатель быстрее
  digitalWrite(motor1Pin1, LOW); 
  digitalWrite(motor1Pin2, HIGH); // Левый двигатель вперед
  digitalWrite(motor2Pin1, LOW); 
  digitalWrite(motor2Pin2, HIGH); // Правый двигатель вперед
  server.send(200);
}
void handleLeftDown() {
  Serial.println("Left-Down");
  analogWrite(enable1Pin, 200); // Левый двигатель быстрее
  analogWrite(enable2Pin, 100); // Правый двигатель медленнее
  digitalWrite(motor1Pin1, HIGH); 
  digitalWrite(motor1Pin2, LOW); // Левый двигатель назад
  digitalWrite(motor2Pin1, HIGH); 
  digitalWrite(motor2Pin2, LOW); // Правый двигатель назад
  server.send(200);
}

void handleForward() {
  Serial.println("Forward");
  analogWrite(enable1Pin, 200); // Set speed for motor 1
  analogWrite(enable2Pin, 200); // Set speed for motor 2
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); // Motor 1 forward
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH); // Motor 2 forward
  server.send(200);
}

void handleReverse() {
  Serial.println("Reverse");
  analogWrite(enable1Pin, 200); // Set speed for motor 1
  analogWrite(enable2Pin, 200); // Set speed for motor 2
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); // Motor 1 reverse
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW); // Motor 2 reverse
  server.send(200);
}

void handleStop() {
  Serial.println("Stop");
  digitalWrite(motor1Pin1, LOW); 
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);   
  server.send(200);
}

void handleLeft() {
  Serial.println("Left");
  analogWrite(enable1Pin, 200); // Set speed for motor 1
  analogWrite(enable2Pin, 200); // Set speed for motor 2
  digitalWrite(motor1Pin1, HIGH); // Motor 1 reverse
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);  // Motor 2 forward
  digitalWrite(motor2Pin2, HIGH);
  server.send(200);
}

void handleRight() {
  Serial.println("Right");
  analogWrite(enable1Pin, 200); // Set speed for motor 1
  analogWrite(enable2Pin, 200); // Set speed for motor 2
  digitalWrite(motor1Pin1, LOW);  // Motor 1 forward
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH); // Motor 2 reverse
  digitalWrite(motor2Pin2, LOW);
  server.send(200);
}


void handleSpeed() {
  if (server.hasArg("value")) {
    valueString = server.arg("value");
    int value = valueString.toInt();
    if (value == 0) {
      analogWrite(enable1Pin, 0); // Stop motor 1
      analogWrite(enable2Pin, 0); // Stop motor 2
      digitalWrite(motor1Pin1, LOW); 
      digitalWrite(motor1Pin2, LOW); 
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW);   
    } else {
      dutyCycle = map(value, 0, 100, 0, 255); // Map to PWM range
      analogWrite(enable1Pin, dutyCycle); // Set motor speed for motor 1
      analogWrite(enable2Pin, dutyCycle); // Set motor speed for motor 2
      Serial.println("Motor speed set to " + String(value));
    }
  }
  server.send(200);
}

void setup() {
  Serial.begin(115200);

  // Set the Motor pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // Initialize PWM with 0 duty cycle using analogWrite
  pinMode(enable1Pin, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  analogWrite(enable1Pin, 0); // Set initial speed to 0
  analogWrite(enable2Pin, 0); // Set initial speed to 0

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Define routes
  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/left", handleLeft);
  server.on("/stop", handleStop);
  server.on("/right", handleRight);
  server.on("/reverse", handleReverse);
  server.on("/speed", handleSpeed);
  server.on("/right-up", handleRightUp);
  server.on("/right-down", handleRightDown);
  server.on("/left-up", handleLeftUp);
  server.on("/left-down", handleLeftDown);

  // Start the server
  server.begin();
}

void loop() {
  server.handleClient();
}
