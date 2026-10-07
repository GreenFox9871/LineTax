/* Layout:
  - M1, M2 -> RIGHT side motors (relative to direction of movement)
  - M3, M4 -> LEFT side motors  (relative to direction of movement)
  - RIGHT IR sensor -> A0
  - LEFT IR sensor  -> A1 */


#include <AFMotor.h>
#define DEBUG_PRINT 0

#define RIGHT_IR A0
#define LEFT_IR  A1

#define DETECT_LIMIT 300
#define FORWARD_SPEED 60
#define TURN_SHARP_SPEED 150
#define TURN_SLIGHT_SPEED 120
#define DELAY_AFTER_TURN 140
#define BEFORE_TURN_DELAY 10

// Right side motors
AF_DCMotor motorR1(1); // M1
AF_DCMotor motorR2(2); // M2
// Left side motors
AF_DCMotor motorL1(3); // M3
AF_DCMotor motorL2(4); // M4

// variables to store the analog values
int left_value;
int right_value;

// Set the last direction to Stop
char lastDirection = 'S';

void setup() {
  if DEBUG_PRINT
  Serial.begin(9600);

  setLeftSpeed(0);
  runLeft(RELEASE);
  setRightSpeed(0);
  runRight(RELEASE);

  // To provide starting push to Robot these values are set
  runRight(FORWARD);
  runLeft(FORWARD);
  setLeftSpeed(255);
  setRightSpeed(255);
  delay(40); // delay of 40 ms
}

void loop() {
  left_value  = analogRead(LEFT_IR);
  right_value = analogRead(RIGHT_IR);

  if DEBUG_PRINT
  Serial.print(left_value);
  Serial.print(",");
  Serial.print(right_value);
  Serial.print(",");
  Serial.print(lastDirection);
  Serial.write(10);

  // Right Sensor detects black line and left does not detect
  if (right_value >= DETECT_LIMIT && !(left_value >= DETECT_LIMIT)) {
    turnRight();
  }
  // Left Sensor detects black line and right does not detect
  else if ((left_value >= DETECT_LIMIT) && !(right_value >= DETECT_LIMIT)) {
    turnLeft();
  }
  // both sensors don't detect black line
  else if (!(left_value >= DETECT_LIMIT) && !(right_value >= DETECT_LIMIT)) {
    moveForward();
  }
  // both sensors detect black line
  else if ((left_value >= DETECT_LIMIT) && (right_value >= DETECT_LIMIT)) {
    stop();
  }
}

void runLeft(uint8_t dir) {
  motorL1.run(dir);
  motorL2.run(dir);
}

void runRight(uint8_t dir) {
  motorR1.run(dir);
  motorR2.run(dir);
}

void setLeftSpeed(uint8_t speed) {
  motorL1.setSpeed(speed);
  motorL2.setSpeed(speed);
}

void setRightSpeed(uint8_t speed) {
  motorR1.setSpeed(speed);
  motorR2.setSpeed(speed);
}

void moveForward() {
  if (lastDirection != 'F') {
    runRight(FORWARD);
    runLeft(FORWARD);
    setLeftSpeed(255);
    setRightSpeed(255);
    lastDirection = 'F';
    delay(20);
  } else {
    runRight(FORWARD);
    runLeft(FORWARD);
    setLeftSpeed(FORWARD_SPEED);
    setRightSpeed(FORWARD_SPEED);
  }
}

void stop() {
  if (lastDirection != 'S') {
    runRight(FORWARD);
    runLeft(FORWARD);
    setLeftSpeed(255);
    setRightSpeed(255);
    lastDirection = 'S';
    delay(40);
  } else {
    setLeftSpeed(0);
    setRightSpeed(0);
    runLeft(RELEASE);
    runRight(RELEASE);
    lastDirection = 'S';
  }
}

void turnRight(void) {
  if (lastDirection != 'R') {
    lastDirection = 'R';
    setLeftSpeed(0);
    setRightSpeed(0);
    delay(BEFORE_TURN_DELAY);
    runLeft(FORWARD);
    runRight(BACKWARD);
    setLeftSpeed(TURN_SLIGHT_SPEED);
    setRightSpeed(TURN_SLIGHT_SPEED);
  } else {
    runLeft(FORWARD);
    runRight(BACKWARD);
    setLeftSpeed(TURN_SHARP_SPEED);
    setRightSpeed(TURN_SHARP_SPEED);
  }
  delay(DELAY_AFTER_TURN);
}

void turnLeft() {
  if (lastDirection != 'L') {
    lastDirection = 'L';
    setLeftSpeed(0);
    setRightSpeed(0);
    delay(BEFORE_TURN_DELAY);
    runRight(FORWARD);
    runLeft(BACKWARD);
    setLeftSpeed(TURN_SLIGHT_SPEED);
    setRightSpeed(TURN_SLIGHT_SPEED);
  } else {
    runRight(FORWARD);
    runLeft(BACKWARD);
    setLeftSpeed(TURN_SHARP_SPEED);
    setRightSpeed(TURN_SHARP_SPEED);
  }
  delay(DELAY_AFTER_TURN);
}