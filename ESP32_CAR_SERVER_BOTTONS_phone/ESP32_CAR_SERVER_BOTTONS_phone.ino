#include <WiFi.h> 
#include <WebServer.h>
#include <ESP32Servo.h>  // Add Servo library

// Replace with your network credentials
const char* ssid     = "Borealis Laboratory";
const char* password = "artofwar3";

// Create an instance of the WebServer on port 80
WebServer server(80);

// Motor 1
int motor1Pin1 = 18;  // Changed pin
int motor1Pin2 = 19;  // Changed pin
int enable1Pin = 22;  // Changed pin (PWM pin for Motor 1)

// Motor 2
int motor2Pin1 = 32;  // Changed pin
int motor2Pin2 = 33;  // Changed pin
int enable2Pin = 25;  // Changed pin (PWM pin for Motor 2)

// Servo pin
int servoPin = 26;  // Pin for the servo motor
Servo myServo;  // Create a Servo object

// Setting PWM properties
int dutyCycle = 0; // Duty cycle for motors

String valueString = String(0);

void handleRoot() {
  const char html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
      html {
        font-family: Helvetica;
        text-align: center;
        background-color: #f0f0f0;
        padding: 10px;
      }
      .container {
        display: grid;
        grid-template-rows: repeat(3, 1fr);
        grid-template-columns: repeat(3, 1fr);
        gap: 10px;
        justify-items: center;
        align-items: center;
        max-width: 90vw;
        margin: auto;
      }
      button {
        font-size: 5vw;
        color: white;
        border: none;
        border-radius: 10px;
        cursor: pointer;
        width: 20vw;
        height: 20vw;
        max-width: 100px;
        max-height: 100px;
        transition: 0.2s;
      }
      button:hover { opacity: 0.8; }

      .red { background-color: #f44336; }
      .red:hover { background-color: #e53935; }

      .blue { background-color: #2196F3; }
      .blue:hover { background-color: #1976D2; }

      .green {
        background-color: #4CAF50;
        font-size: 4vw;
        width: 60vw;
        max-width: 200px;
        height: 12vw;
        max-height: 50px;
      }
      .green:hover { background-color: #45a049; }

      @media (max-width: 480px) {
        button {
          font-size: 7vw;
          width: 25vw;
          height: 25vw;
        }
        .green {
          font-size: 5vw;
          height: 15vw;
        }
      }
    </style>
  </head>
  <body>
    <h1>ESP32 Motor Control</h1>
    <div class="container">
      <button class="blue" ontouchstart="sendCommand('left-up')" ontouchend="sendCommand('stop')">LU</button>
      <button class="red" ontouchstart="sendCommand('forward')" ontouchend="sendCommand('stop')">F</button>
      <button class="blue" ontouchstart="sendCommand('right-up')" ontouchend="sendCommand('stop')">RU</button>
      <button class="red" ontouchstart="sendCommand('left')" ontouchend="sendCommand('stop')">L</button>
      <button class="red" ontouchstart="sendCommand('stop')">S</button>
      <button class="red" ontouchstart="sendCommand('right')" ontouchend="sendCommand('stop')">R</button>
      <button class="blue" ontouchstart="sendCommand('left-down')" ontouchend="sendCommand('stop')">LD</button>
      <button class="red" ontouchstart="sendCommand('reverse')" ontouchend="sendCommand('stop')">Rev</button>
      <button class="blue" ontouchstart="sendCommand('right-down')" ontouchend="sendCommand('stop')">RD</button>
    </div>
    <button class="green" onclick="sendCommand('move-servo')">Hit Ball</button>
    <script>
      function sendCommand(command) {
        fetch('/' + command);
      }
    </script>
  </body>
  </html>



)rawliteral";
  server.send(200, "text/html", html);
}

void handleServoMovement() {
  Serial.println("Moving Servo");
  myServo.writeMicroseconds(1000);  // Move servo to maximum position
  delay(1000);  // Wait for 1 second
  myServo.write(0);  // Move servo to minimum position
  delay(1000);  // Wait for 1 second
  server.send(200);
}

void handleLeftUp() {
  Serial.println("Right-Up");
  analogWrite(enable1Pin, 200); // Левый двигатель быстрее
  analogWrite(enable2Pin, 100); // Правый двигатель медленнее
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); // Motor 1 reverse
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW); // Motor 2 reverse
  server.send(200);
}
void handleRightDown() {
  Serial.println("Right-Down");
  analogWrite(enable1Pin, 100); // Левый двигатель медленнее
  analogWrite(enable2Pin, 200); // Правый двигатель быстрее
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); // Motor 1 forward
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH); // Motor 2 forward
  server.send(200);
}
void handleRightUp() {
  Serial.println("Left-Up");
  analogWrite(enable1Pin, 100); // Левый двигатель медленнее
  analogWrite(enable2Pin, 200); // Правый двигатель быстрее
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); // Motor 1 reverse
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW); // Motor 2 reverse
  server.send(200);
}
void handleLeftDown() {
  Serial.println("Left-Down");
  analogWrite(enable1Pin, 200); // Левый двигатель быстрее
  analogWrite(enable2Pin, 100); // Правый двигатель медленнее
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); // Motor 1 forward
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH); // Motor 2 forward
  server.send(200);
}

void handleForward() {
  Serial.println("Forward");
  analogWrite(enable1Pin, 200); // Set speed for motor 1
  analogWrite(enable2Pin, 200); // Set speed for motor 2

  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); // Motor 1 reverse
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW); // Motor 2 reverse
  server.send(200);
}

void handleReverse() {
  Serial.println("Reverse");
  analogWrite(enable1Pin, 200); // Set speed for motor 1
  analogWrite(enable2Pin, 200); // Set speed for motor 2
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); // Motor 1 forward
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH); // Motor 2 forward
  server.send(200);
}

void handleStop() {
    Serial.println("Stop");
    for (int speed = dutyCycle; speed >= 0; speed -= 20) {
        analogWrite(enable1Pin, speed);
        analogWrite(enable2Pin, speed);
        delay(10);
    }
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

  // Initialize servo
  myServo.attach(servoPin);

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
  server.on("/right-up", handleRightUp);
  server.on("/right-down", handleRightDown);
  server.on("/left-up", handleLeftUp);
  server.on("/left-down", handleLeftDown);
  server.on("/move-servo", handleServoMovement); 
  // Start the server
  server.begin();
}

void loop() {
  server.handleClient();
}