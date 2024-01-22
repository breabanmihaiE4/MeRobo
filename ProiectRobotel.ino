#include "MeBoard101.h"

MeInfraredReceiver receiver(PORT_8);
MeUltrasonicSensor distanceSensor(PORT_4);
MeLineFollower followerSensor(PORT_3);
MeDCMotor leftMotor(PORT_9);
MeDCMotor rightMotor(PORT_2);

const int NO_MODE = 0;
const int MODE_A = 1;
const int MODE_B = 2;
const int MODE_C = 3;
const int MODE_D = 4;

const int BUTTON_A = 69;
const int BUTTON_B = 70;
const int BUTTON_C = 71; 
const int BUTTON_D = 68;
const int BUTTON_E = 67;
const int BUTTON_F = 13;

const int BUTTON_UP = 64;
const int BUTTON_DOWN = 25;
const int BUTTON_LEFT = 7;
const int BUTTON_RIGHT = 9;

const int BUTTON_0 = 22;
const int BUTTON_1 = 12;
const int BUTTON_2 = 24;
const int BUTTON_3 = 94;
const int BUTTON_4 = 8;

const int SPEED_0 = 0;
const int SPEED_1 = 50;
const int SPEED_2 = 100;
const int SPEED_3 = 200;
const int SPEED_4 = 400;

const int NO_MOVEMENT = 0;
const int SMART_MOVEMENT = 1;
const int DUMB_MOVEMENT = 2;

int currentRunningMode = NO_MODE;
int autoMovementType = NO_MOVEMENT;
int motorsSpeed = SPEED_0;

void setup() {
  Serial.begin(9600);
  receiver.begin();
}

void loop() {
  switch (currentRunningMode) {
    case MODE_A:
      runModeA();
      break;

    case MODE_B:
      runModeB();
      break;

    case MODE_C:
      runModeC();
      break;

    case MODE_D:
      runModeD();
      break;

    default:
      break;
  }

  if (receiver.buttonState()) {
    switch (receiver.read()) {  
      case BUTTON_A:
        Serial.println("Mode A \n");
        currentRunningMode = MODE_A;
        resetMotors();
        break;

      case BUTTON_B:
        Serial.println("Mode B \n");
        currentRunningMode = MODE_B;
        resetMotors();
        break;

      case BUTTON_C:
        Serial.println("Mode C \n");
        currentRunningMode = MODE_C;
        resetMotors();
        break;

      case BUTTON_D:
        Serial.println("Mode D \n");
        currentRunningMode = MODE_D;
        resetMotors();
        break;

      default:
        Serial.println(receiver.read());
        break;
    }
  }
}

void runModeA() {
  switch(followerSensor.readSensors()) {
    case 0:
      moveForward(SPEED_2);
      break;

    case 1:
      leftMotor.run(SPEED_3);
      rightMotor.run(SPEED_3);
      break;
    
    case 2:
      leftMotor.run(-SPEED_3);
      rightMotor.run(-SPEED_3);
      break;

    default:
      moveBackward(SPEED_2);
      break;
  }
}

void runModeB() {
  switch (autoMovementType) {
    case SMART_MOVEMENT:
      startSmartMovement();
      break;

    case DUMB_MOVEMENT:
      startDumbMovement();
      break;

    default:
      resetMotors();
      break;
  }

  if(receiver.buttonState()) {
    int command = receiver.read();

    switch (command) {
      case BUTTON_0:
        resetMotors();
        autoMovementType = NO_MOVEMENT;
        break;

      case BUTTON_1:
        resetMotors();
        autoMovementType = SMART_MOVEMENT;
        moveForward(SPEED_2);
        break;

      case BUTTON_2:
        resetMotors();
        autoMovementType = DUMB_MOVEMENT;
        moveForward(SPEED_2);
        break;

      default:
        break;
    }
  } 
}

void runModeC() {
  if(receiver.buttonState()) {
    int comand = receiver.read();
    switch (comand) {
      case BUTTON_UP:
        moveForward(motorsSpeed);
        break;

      case BUTTON_DOWN: 
        moveBackward(motorsSpeed);
        break;

      case BUTTON_LEFT:
        moveLeft(motorsSpeed);
        break;

      case BUTTON_RIGHT:
        moveRight(motorsSpeed);
        break;

      case BUTTON_E:
        leftMotor.run(-motorsSpeed);
        rightMotor.run(-motorsSpeed);
        break;

      case BUTTON_F:
        leftMotor.run(motorsSpeed);
        rightMotor.run(motorsSpeed);
        break;

      case BUTTON_0: 
        motorsSpeed = SPEED_0;
        break;

      case BUTTON_1:
        motorsSpeed = SPEED_1;
        break;

      case BUTTON_2:
        motorsSpeed = SPEED_2;
        break;
      
      case BUTTON_3:
        motorsSpeed = SPEED_3;
        break;
      
      case BUTTON_4:
        motorsSpeed = SPEED_4;
        break;

      default:
        break;
    }
  } else {
    resetMotors();
  }
}

void runModeD() {
  if(receiver.buttonState()) {
    int comand = receiver.read();
    switch (comand) {
      case BUTTON_0: 
        drawCircle();
        break;

      case BUTTON_1:
        drawSquare();
        break;

      case BUTTON_2:
        drawTriangle();
        break;
      
      case BUTTON_3:
        drawStar();
        break;
      
      case BUTTON_4:
        drawLoop();
        break;

      default:
        break;
    }
  } else {
    resetMotors();
  }
}

void startSmartMovement() {
  int maxDistance = 0;
  int maxDegrees = 0;

  if (distanceSensor.distanceCm() < 20) {
    resetMotors();
    maxDistance = 0;
    maxDegrees = 0;
    
    for(int degrees = 0; degrees < 13; degrees++) {
      delay(100);
      rotateRight30();

      int distance = distanceSensor.distanceCm();

      if (distance >= maxDistance && distance < 400) {
        maxDistance = distance;
        maxDegrees = degrees;
      }
    }

    delay(500);

    for(int degrees = 0; degrees < maxDegrees; degrees++) {
      rotateRight30();
    }
    moveForward(SPEED_2);
  }
}

void startDumbMovement() {
  if (distanceSensor.distanceCm() < 20) {
    resetMotors();
    
    int randomDirection = random(101);
    
    if(randomDirection % 2 == 0) {
      rotateLeft90();
    } else {
      rotateRight90();
    }
    
    delay(500);

    moveForward(SPEED_2);
  }
}

void resetMotors() {
  leftMotor.run(0);
  rightMotor.run(0);
}

void moveForward(int speed) {
  leftMotor.run(-speed);
  rightMotor.run(speed);
}

void moveBackward(int speed) {
  leftMotor.run(speed);
  rightMotor.run(-speed);
}

void moveLeft(int speed) {
  leftMotor.run(-speed / 2);
  rightMotor.run(speed);
}

void moveRight(int speed) {
  leftMotor.run(-speed);
  rightMotor.run(speed / 2);
}

void rotateLeft90() {
    leftMotor.run(SPEED_2);
    rightMotor.run(SPEED_2);
    delay(425);
    resetMotors();
}

void rotateRight90() {
    leftMotor.run(-SPEED_2);
    rightMotor.run(-SPEED_2);
    delay(425);
    resetMotors();
}

void rotateRight30() {
    leftMotor.run(-SPEED_2);
    rightMotor.run(-SPEED_2);
    delay(155);
    resetMotors();
}

void drawCircle() {
  moveRight(SPEED_3);
  delay(2850);
  resetMotors();
}

void drawSquare() {
  for(int i = 0; i < 4; i++) {
    moveForward(SPEED_2);
    delay(2000);
    resetMotors();
    rotateRight90();
  }
}

void drawTriangle() {
  for(int i = 0; i < 3; i++) {
    moveForward(SPEED_2);
    delay(2000);
    resetMotors();
    rotateRight30();
    rotateRight90();
  }
} 

void drawStar() {
  for(int i = 0; i < 5; i++) {
    moveForward(SPEED_2);
    delay(2000);
    resetMotors();
    leftMotor.run(-SPEED_2);
    rightMotor.run(-SPEED_2);
    delay(250);
    resetMotors();
    rotateRight90();
  }
}

void drawLoop() {
  for(int i = 0; i < 2; i++) {
    moveForward(SPEED_3);
    delay(800);

    resetMotors();
    
    moveRight(SPEED_3);
    delay(2200);

    resetMotors();

    moveForward(SPEED_3);
    delay(800);

    resetMotors();
    
    moveLeft(SPEED_3);
    delay(2200);

    resetMotors();
  }
}
