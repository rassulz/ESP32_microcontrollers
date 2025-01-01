#include <AccelStepper.h>

// Define motor interface type and pins
#define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
#define STEP_PIN 14   // Pin connected to the step input
#define DIR_PIN 12    // Pin connected to the direction input

AccelStepper stepper(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);

const float STEPS_PER_REV = 200.0;  // Number of steps per full revolution (adjust for your motor)
float currentAngle = 0.0;           // Track the motor's current angle (0 to 360 degrees)

void setup() {
  Serial.begin(115200);
  stepper.setMaxSpeed(1000);   // Set maximum speed (steps per second)
  stepper.setAcceleration(500); // Set acceleration (steps per second^2)
  Serial.println("Stepper motor ready. Use command: A<angle> (0-360)");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');  // Read input until newline
    input.trim();  // Remove any whitespace

    if (input.startsWith("A")) {
      // Extract the target angle
      float targetAngle = input.substring(1).toFloat();

      // Validate the target angle (0-360 degrees)
      if (targetAngle < 0.0 || targetAngle > 360.0) {
        Serial.println("Invalid angle! Enter a value between 0 and 360.");
        return;
      }

      // Calculate the shortest path to the target angle
      float angleDifference = targetAngle - currentAngle;

      // Handle wrap-around (e.g., 350 to 10 degrees)
      if (angleDifference > 180.0) {
        angleDifference -= 360.0;
      } else if (angleDifference < -180.0) {
        angleDifference += 360.0;
      }

      // Convert the angle difference to steps
      int steps = (angleDifference / 360.0) * STEPS_PER_REV;

      // Move the motor by the calculated steps
      stepper.move(steps);

      // Execute the motion
      while (stepper.distanceToGo() != 0) {
        stepper.run();
      }

      // Update the current angle
      currentAngle = targetAngle;

      // Print the new position
      Serial.print("Moved to angle: ");
      Serial.println(currentAngle);
    } else {
      Serial.println("Invalid command! Use format: A<angle> (e.g., A90)");
    }
  }
}
