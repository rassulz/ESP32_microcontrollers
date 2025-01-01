
#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid     = "Borealis Laboratory";
const char* password = "artofwar3";

// Create an instance of the WebServer on port 80
WebServer server(80);

// Motor 1
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14;

// Motor 2
int motor2Pin1 = 33; 
int motor2Pin2 = 25; 
int enable2Pin = 32;

// Setting PWM properties
const int freq = 30000;
const int resolution = 8;
int dutyCycle = 0;

String valueString = String(0);

void handleRoot() {
  const char html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
      html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; }
      #joystick { width: 100px; height: 100px; background-color: #4CAF50; border-radius: 50%; margin: 20px auto; position: relative; touch-action: none; }
      #knob { width: 40px; height: 40px; background-color: #555555; border-radius: 50%; position: absolute; top: 30px; left: 30px; }
      #motorSpeedDisplay { margin-top: 20px; }
    </style>
    <script>
      let joystickCenter = { x: 50, y: 50 };
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
        const distance = Math.min(Math.sqrt(x * x + y * y), 50);
        knob.style.left = `${50 + distance * Math.cos(angle) - 20}px`;
        knob.style.top = `${50 + distance * Math.sin(angle) - 20}px`;

        if (distance > 20) {
          const direction = getDirection(angle);
          if (direction !== lastDirection) {
            lastDirection = direction;
            fetch(`/${direction}`);
          }
        }
      }

      function getDirection(angle) {
        if (angle >= -Math.PI / 4 && angle < Math.PI / 4) return "right";
        if (angle >= Math.PI / 4 && angle < 3 * Math.PI / 4) return "forward";
        if (angle >= -3 * Math.PI / 4 && angle < -Math.PI / 4) return "reverse";
        return "left";
      }

      function resetKnob() {
        const knob = document.getElementById("knob");
        knob.style.left = "30px";
        knob.style.top = "30px";
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


void handleForward() {
  Serial.println("Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  server.send(200);
}

void handleLeft() {
  Serial.println("Left");
  digitalWrite(motor1Pin1, HIGH); 
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
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

void handleRight() {
  Serial.println("Right");
  digitalWrite(motor1Pin1, LOW); 
  digitalWrite(motor1Pin2, HIGH); 
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);    
  server.send(200);
}

void handleReverse() {
  Serial.println("Reverse");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);          
  server.send(200);
}

void handleSpeed() {
  if (server.hasArg("value")) {
    valueString = server.arg("value");
    int value = valueString.toInt();
    if (value == 0) {
      ledcWrite(enable1Pin, 0);
      ledcWrite(enable2Pin, 0);
      digitalWrite(motor1Pin1, LOW); 
      digitalWrite(motor1Pin2, LOW); 
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW);   
    } else { 
      dutyCycle = map(value, 25, 100, 200, 255);
      ledcWrite(enable1Pin, dutyCycle);
      ledcWrite(enable2Pin, dutyCycle);
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

  // Configure PWM Pins
  ledcAttach(enable1Pin, freq, resolution);
  ledcAttach(enable2Pin, freq, resolution);
    
  // Initialize PWM with 0 duty cycle
  ledcWrite(enable1Pin, 0);
  ledcWrite(enable2Pin, 0);
  
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

  // Start the server
  server.begin();
}

void loop() {
  server.handleClient();
}