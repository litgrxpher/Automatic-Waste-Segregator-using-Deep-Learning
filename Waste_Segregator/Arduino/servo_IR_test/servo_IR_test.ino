#include <Servo.h>

Servo trapdoor;  // Create a servo object to control the trapdoor servo
const int irSensorPin = 2;  // Pin for the IR sensor

void setup() {
  Serial.begin(9600);
  trapdoor.attach(3);  // Attach the servo to pin 3
  pinMode(irSensorPin, INPUT);

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
    char command = Serial.read(); // Read a single character from serial
    // If command is '1', open trapdoor
    if (command == '1') {
      openTrapdoor();
    }
  }
}

void initializeServo() {
  // Initialize the servo to position 0 degrees
  trapdoor.write(0);
  delay(1000);  // Wait for servo to reach position
}

void openTrapdoor() {
  // Gradually open the trapdoor using the servo motor
  for (int angle = 0; angle <= 90; angle++) {
    trapdoor.write(angle); // Incrementally increase the angle
    delay(20); // Adjust the delay to control the speed (smaller delay -> slower speed)
  }
  
  // Gradually close the trapdoor using the servo motor
  for (int angle = 90; angle >= 0; angle--) {
    trapdoor.write(angle); // Decrementally decrease the angle
    delay(20); // Adjust the delay to control the speed (smaller delay -> slower speed)
  }
  
  // Send acknowledgement to Python that operation is complete
  Serial.println("Done");
}


