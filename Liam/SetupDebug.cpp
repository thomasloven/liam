/*
 Liam - DIY Robot Lawn Mower

 Debug Mode Library

 ======================
  Licensed under GPLv3
 ======================
*/

#include "SetupDebug.h"
#include "Definition.h"

SETUPDEBUG::SETUPDEBUG(CONTROLLER* controller, WHEELMOTOR* left, WHEELMOTOR* right, CUTTERMOTOR* cut, BWFSENSOR* bwf, MOTIONSENSOR* comp, BATTERY* batt) {
  mower = controller;
  leftMotor = left;
  rightMotor = right;
  cutter = cut;
  sensor = bwf;
  compass = comp;
  battery = batt;
}

int SETUPDEBUG::tryEnterSetupDebugMode(int currentState) {

  char inChar;

  if (currentState != SETUP_DEBUG) {
    if (Serial.available()) {
      inChar = (char)Serial.read();
      if (inChar == 'd' || inChar == 'D') {
        mower->stopCutter();
        mower->stop();
        SETUPDEBUG::printHelp();
        return SETUP_DEBUG;
      }
    }
    return currentState;
  }

  while (!Serial.available());      // Stay here until data is available
  inChar = (char)Serial.read(); // get the new byte:

  switch (inChar) {
    case 'H':
    case 'h':
      SETUPDEBUG::printHelp();
      break;

    case 'D':
    case 'd':
      SETUPDEBUG::toggleLed();
      break;

    case 'L':
    case 'l':
      SETUPDEBUG::toggleWheelLeft();
      break;

    case 'R':
    case 'r':
      SETUPDEBUG::togglewheelRight();
      break;

    case 'S':
    case 's':
      SETUPDEBUG::getBwfSignals();
      break;

    case 'C':
    case 'c':
      SETUPDEBUG::toggleCutterMotor();
      break;

    case 'T':
    case 't':
      SETUPDEBUG::testRun();
      break;

    case '+':
      SETUPDEBUG::cutterSpeedUp();
      break;

    case '-':
      SETUPDEBUG::cutterSpeedDown();
      break;

    case 'P':
    case 'p':
      SETUPDEBUG::printStatus();
      break;

    case 'g':
    case 'G':
      SETUPDEBUG::getMotionValues();
      break;
    case 'm':
    case 'M':
      mower->runForward(FULLSPEED);
      return MOWING;
    case 'b':
    case 'B':
      return LOOKING_FOR_BWF;

    case 'a':
    case 'A':
      SETUPDEBUG::trimpotAdjust();
      break;
  }

  inChar = 0;

return SETUP_DEBUG;
}

void SETUPDEBUG::printHelp() {
  Serial.println(F("------- Help menu ------------"));
  Serial.println(F("L = Left Wheel motor on/off"));
  Serial.println(F("R = Right Wheel motor on/off"));
  Serial.println(F("C = Cutter motor on/off"));
  Serial.println(F("S = test BWF Sensor"));
  Serial.println(F("G = test Gyro/Compass/Accelerometer"));
  Serial.println(F("D = turn LED on/off"));
  Serial.println(F("T = make a 10 second test run"));
  Serial.println(F("P = print battery & debug values"));
  Serial.println(F("A = Trimpot adjust mode"));
  Serial.println(F("M = Start mowing"));
  Serial.println(F("B = Look for BWF and dock"));
}

void SETUPDEBUG::toggleLed() {

  if (led_is_on)
    digitalWrite(10,LOW);
  else
    digitalWrite(10,HIGH);

  led_is_on = !led_is_on;
}

void SETUPDEBUG::toggleWheelLeft() {
  if (left_wheel_motor_is_on == true) {
    Serial.println(F("Ramping down left wheel"));
    for (int i=100; i>0; i--) {
      leftMotor->setSpeed(i);
      delay(10);
    }
    Serial.println(F("Ramp down completed"));
    left_wheel_motor_is_on = false;
  } else {
    Serial.println(F("Ramping up left wheel"));
    for (int i=0; i<100; i++) {
      leftMotor->setSpeed(i);
      delay(10);
    }
    Serial.println(F("Ramp up completed"));
    left_wheel_motor_is_on = true;
  }
}

void SETUPDEBUG::togglewheelRight() {
  if (right_wheel_motor_is_on == true) {
    Serial.println(F("Ramping down right wheel"));
    for (int i=100; i>0; i--) {
      rightMotor->setSpeed(i);
      delay(10);
    }
    Serial.println(F("Ramp down completed"));
    right_wheel_motor_is_on = false;
  } else {
    Serial.println(F("Ramping up right wheel"));
    for (int i=0; i<100; i++) {
      rightMotor->setSpeed(i);
      delay(10);
    }
    Serial.println(F("Ramp up completed"));
    right_wheel_motor_is_on = true;
  }
}

void SETUPDEBUG::getBwfSignals() {
  Serial.println(F("-------- Testing Sensors 0 -> 3 --------"));
  for (int i=0; i<4; i++) {
    sensor->select(i);
    delay(1000);
    Serial.print(i);
    Serial.print(F(": "));
    sensor->printSignal();
    Serial.print(F(" in:"));
    Serial.print(sensor->isInside());
    Serial.print(F(" out:"));
    Serial.print(sensor->isOutside());
    Serial.println();
  }
  Serial.println(F("Sensor test completed"));
}

void SETUPDEBUG::toggleCutterMotor() {
  if (cutter_motor_is_on) {
    Serial.println(F("Ramping down cutter"));
    for (int i=100; i>=0; i--) {
      cutter->setSpeed(i);
      delay(10);
    }
    Serial.println(F("Ramp down completed"));
    cutterspeed = 0;
    cutter_motor_is_on = false;
  }
  else
  {
    Serial.println(F("Ramping up cutter"));
    for (int i=0; i<100; i++) {
      cutter->setSpeed(i);
      delay(10);
    }
    Serial.println(F("Ramp up completed"));
    cutterspeed = 100;
    cutter_motor_is_on = true;
  }
}

void SETUPDEBUG::testRun() {
  for (int i=0; i<100; i++) {
    sensor->select(0);
    delay(100);
    rightMotor->setSpeed((sensor->isInside()?100:-100));

    sensor->select(1);
    delay(100);
    leftMotor->setSpeed((sensor->isInside()?100:-100));
  }
  leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
}

void SETUPDEBUG::cutterSpeedUp() {
  cutterspeed += 10;
  if(cutterspeed > 100) cutterspeed = 100;
  cutter->setSpeed(cutterspeed);
  Serial.println(cutterspeed);
}

void SETUPDEBUG::cutterSpeedDown() {
  cutterspeed -= 10;
  if(cutterspeed < 0) cutterspeed = 0;
  cutter->setSpeed(cutterspeed);
  Serial.println(cutterspeed);
}

void SETUPDEBUG::printStatus() {
  Serial.print(F(" LMot: "));
  Serial.print(leftMotor->getLoad());
  Serial.print(F(" RMot: "));
  Serial.print(rightMotor->getLoad());
  Serial.print(F(" BAT: "));
  battery->resetVoltage();
  Serial.print(battery->getVoltage());
  Serial.print(F(" Dock: "));
  Serial.print(battery->isBeingCharged());
}

void SETUPDEBUG::getMotionValues() {
#if __MMA7455__
  compass->autoupdate();
#endif

  int tilt_angle = compass->getTiltAngle();

  int x = compass->getXAngle();
  int y = compass->getYAngle();
  int z = compass->getZAngle();

  Serial.print(F("Z = "));
  Serial.println(z);
  Serial.print(F("Y = "));
  Serial.println(y);
  Serial.print(F("X = "));
  Serial.println(x);

  Serial.print(F("Tilt angle = "));
  Serial.println(tilt_angle);
}

void SETUPDEBUG::trimpotAdjust()
{
  Serial.println(F("Continuously printing battery voltage"));
  Serial.println(F("Adjust your pot to match measured value"));
  Serial.println(F("Send D to return to debug mode"));
  Serial.print(F("Starting in 5"));
  delay(1000);
  Serial.print(F("...4"));
  delay(1000);
  Serial.print(F("...3"));
  delay(1000);
  Serial.print(F("...2"));
  delay(1000);
  Serial.println(F("...1"));
  delay(1000);
  while(1)
  {
    while(Serial.available())
    {
      char inChar = (char)Serial.read();
      if (inChar == 'd' || inChar == 'D')
      {
        Serial.println(F("Returning to debug mode"));
        return;
      }
    }

    battery->resetVoltage();
    Serial.print(battery->getVoltage());
    Serial.println(F("     (D to stop)"));
    delay(500);
  }
}

