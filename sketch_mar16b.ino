
#include <Servo.h>        // Include Servo library
#include <AccelStepper.h> // Include AccelStepper library

// Define connections for stepper motor
const int stepPin = 3;     
const int dirPin = 4;
const int enablePin = 8;

// Define connections for servos
const int servo1Pin = 5;
const int servo2Pin = 6;

// Define connection for button
const int buttonPin = 2;

// Define speed control potentiometer
const int speedPotPin = A0;

// Define duration control potentiometer
const int durationPotPin = A1;

// Create servo objects
Servo servo1;
Servo servo2;

// Create stepper motor object
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

// Define variables for motor speed and duration
int motorSpeed;
int motorDuration;

// Define variables for button and state
int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int currentState = 0;
int nextState = 1;

void setup() {
  // Initialize stepper motor pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Initialize servo motor pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Initialize button pin
  pinMode(buttonPin, INPUT_PULLUP);

  // Set initial state for stepper motor
  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(2000);
  stepper.setEnablePin(enablePin);
  stepper.disableOutputs();
}

void loop() {
  // Read speed control potentiometer
  motorSpeed = analogRead(speedPotPin);
  motorSpeed = map(motorSpeed, 0, 1023, 0, 5000);

  // Read duration control potentiometer
  motorDuration = analogRead(durationPotPin);
  motorDuration = map(motorDuration, 0, 1023, 1000, 5000);

  // Read button state and debounce
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        currentState = nextState;
      }
    }
  }
  lastButtonState = reading;

  // Execute current state
  switch (currentState) {
    case 0:
      // Reset to initial state
      servo2.write(0);
      delay(1000);
      servo1.write(0);
      nextState = 1;
      break;

    case 1:
      // Move servo2 by 90 degrees clockwise
      servo2.write(90);
      delay(1000);
      nextState = 2;
      break;

    case 2:
      // Move servo1 by 100 degrees clockwise
      servo1.write(100);
      delay(1500);
      nextState = 3;
      break;

    case 3:
      // Start stepper motor with speed and duration set by potentiometers
      stepper.enableOutputs();
      stepper.setSpeed(motorSpeed);
      stepper.move(200);
      while (stepper.distanceToGo() != 0) {
        stepper.run();
      }
      delay(motorDuration);
      stepper.disableOutputs();
  nextState = 4;
  break;

case 4:
  // Stop stepper motor and wait for one second
  stepper.stop();
  stepper.disableOutputs();
  delay(1000);
  nextState = 5;
  break;

case 5:
  // Move servo2 by 90 degrees counterclockwise
  servo2.write(0);
  delay(1000);
  nextState = 6;
  break;

case 6:
  // Move servo1 by 100 degrees counterclockwise
  servo1.write(0);
  delay(1000);
  nextState = 0;
  break;

default:
  // Reset to initial state if unknown state detected
  currentState = 0;
  break;

  }
}

// Bitte beachten Sie, dass dies nur ein Beispielcode ist und je nach Ihren spezifischen Anforderungen möglicherweise angepasst werden muss. Außerdem wurde der Code nicht getestet und es ist möglich, dass noch Fehler vorhanden sind. Bitte überprüfen Sie den Code sorgfältig und testen Sie ihn gründlich, bevor Sie ihn in einem realen Projekt einsetzen.