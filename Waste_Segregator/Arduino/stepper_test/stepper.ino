#include <Stepper.h>

// Define the number of steps per revolution
const int stepsPerRevolution = 200;

// Initialize the stepper motor object with the number of steps and motor pins
Stepper stepper(stepsPerRevolution, 8, 9, 10, 11); // Motor pins connected to L293D pins 1, 2, 3, 4

void setup() {
  // Set the speed of the stepper motor (RPM)
  stepper.setSpeed(60); // Change speed as needed (in RPM)
}

void loop() {
  // Rotate the stepper motor continuously in the clockwise direction
  stepper.step(stepsPerRevolution);
}
