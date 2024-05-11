#include <Stepper.h>
#include <Servo.h>

Servo trapdoor;  // Create a servo object to control the trapdoor servo
const int irSensorPin = 2;  // Pin for the IR sensor
const int stepsPerRevolution = 200;

// Define stepper motor position limits for each compartment
float compartment[] = {0, 200.0 / 4, 2 * (200.0 / 4), 3 * (200.0 / 4)}; // Adjust these values as per your setup

// Create stepper motor object
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

// Variable to store current stepper position
int currentPosition = 0;

void setup() {
  Serial.begin(9600);
  trapdoor.attach(3);  // Attach the servo to pin 3
  pinMode(irSensorPin, INPUT);
  myStepper.setSpeed(60);

  // Initialize the servo to position 0 degrees
  initializeServo();
}

void loop() {
  // Read the IR sensor value
  int irSensorValue = digitalRead(irSensorPin);

  // If object detected by IR sensor
  if (irSensorValue == LOW) {
    // Send "detect" command to Python
    Serial.println("detect");
    delay(1000); // Wait a bit to prevent continuous readings
  }

  // Listen for commands from Python
  if (Serial.available() > 0) {
    String predictedClass = Serial.readStringUntil('\n');
    // If command is a predicted class, move stepper motor
    moveStepper(predictedClass);
  }
}

void initializeServo() {
  // Initialize the servo to position 0 degrees
  trapdoor.write(3);
  delay(1000);  // Wait for servo to reach position
}

void moveStepper(String predictedClass) {
  // Determine compartment based on predicted waste class
  int compartmentIndex = 0;
  if (predictedClass == "Metal") {
    compartmentIndex = 0;
  } else if (predictedClass == "Organic") {
    compartmentIndex = 1;
  } else if (predictedClass == "Paper") {
    compartmentIndex = 2;
  } else if (predictedClass == "Plastic") {
    compartmentIndex = 3;
  }
  
  // Calculate the steps needed to move
  int stepsToMove = compartment[compartmentIndex] - currentPosition;

  // Move to the corresponding compartment
  myStepper.step(stepsToMove);

  // Update the current position
  currentPosition = compartment[compartmentIndex];
  
  // Open the trapdoor
  openTrapdoor();
}

void openTrapdoor() {
  // Gradually open the trapdoor using the servo motor
  for (int angle = 3; angle <= 90; angle++) {
    trapdoor.write(angle); // Incrementally increase the angle
    delay(10); // Adjust the delay to control the speed (smaller delay -> slower speed)
  }
  
  // Gradually close the trapdoor using the servo motor
  for (int angle = 90; angle >= 3; angle--) {
    trapdoor.write(angle); // Decrementally decrease the angle
    delay(20); // Adjust the delay to control the speed (smaller delay -> slower speed)
  }
  
  // Send acknowledgement to Python that operation is complete
  Serial.println("Done");
}
