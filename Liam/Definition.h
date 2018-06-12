/*
 Liam - DIY Robot Lawn Mower

 Default Definitions

 ======================
  Licensed under GPLv3
 ======================
*/

/*
 Note that settings in this file can be overridden in a file
 named `LocalDefinition.h` which is not tracked by git.
*/

#include <Arduino.h>
#include "Wheelmotor.h"
#include "CutterMotor.h"
#include "Battery.h"

#ifndef _DEFINITION_H_
#define _DEFINITION_H_


// Cutter motor types
#define BRUSHLESS  0
#define BRUSHED    1
#define NIDEC      2

#define CUTTER_MOTOR_TYPE BRUSHED

// Battery
#define LEAD_ACID  0
#define NIMH       1
#define LIION      2

#define BATTERY_TYPE LIION

#define DEBUG_ENABLED true

// If not defined, mower will not try to dock, but will keep mowing until battery runs out
#define AUTO_DOCKING

const int NUMBER_OF_SENSORS = 2;  // Number of BWF sensors can be 1-4 depending on shield default 2 left and right front.

// Pin setup following Morgan 1.5 shield and up
#define WHEEL_MOTOR_A_CURRENT_PIN       0
#define WHEEL_MOTOR_B_CURRENT_PIN       1
#define BAT_PIN                         2
#define CUTTER_CURRENT_PIN              3
#define I2C_SDA_PIN                     4
#define I2C_SDL_PIN                     5
// Digital pins
#define RX_PIN                          0
#define TX_PIN                          1
#define BWF_SENSOR_INPUT_PIN            2
#define WHEEL_MOTOR_A_PWM_PIN           3
#define BWF_SELECT_A_PIN                4
#define DOCK_PIN                        5
#define CUTTER_PWM_PIN                  6
#define BWF_SELECT_B_PIN                7
#define BUMPER                          8
#define LIFT_SENSOR_PIN                 9
#define LED_PIN                         13
#define WHEEL_MOTOR_B_PWM_PIN           11
#define WHEEL_MOTOR_A_DIRECTION_PIN     12
#define WHEEL_MOTOR_B_DIRECTION_PIN     13


// Wheel motor
#define WHEELMOTOR_OVERLOAD   130
#define WHEELMOTOR_SMOOTHNESS 300

// CUTTER
#define CUTTER_OVERLOAD     100
#define CUTTER_SPINUP_TIME 4000

// Cutter states
const int MOWING = 0;
const int LAUNCHING = 1;
const int DOCKING = 2;
const int CHARGING = 3;
const int LOOKING_FOR_BWF = 4;
const int SETUP_DEBUG = 5;

// Mower commands
#define CMD_AUTO  0
#define CMD_MOW   1
#define CMD_HOME  2
#define CMD_STOP  3
#define CMD_DEBUG 4

// Turning details
#define REVERSE_DELAY         2
#define TURNDELAY           20 //Reduce for smaller turning angle
// Max expected time between turns
#define TURN_INTERVAL         60000

#pragma region BWF

// BWF Detection method (true = always, false = only at wire)
#define BWF_DETECTION_ALWAYS      true
#define TIMEOUT             6000 //Time without signal before error
#define BWF_COLLECT_SIGNAL_TIME   200 // max time to spend looking for signal
#define BWF_NUMBER_OF_PULSES  3
// Trigger value for the mower to leave the BWF when going home
// The higher value the more turns (in the same direction) the mower can make before leaving
#define BALANCE_TRIGGER_LEVEL     10000

// BWF Code for inside and outside
#define INSIDE_BWF          85
#define OUTSIDE_BWF         5

#pragma endregion BWF


#define MAJOR_VERSION           5
#define MINOR_VERSION_1         2
#define MINOR_VERSION_2         2

// A bit of macro magic to make a string out of the version number
// The preprocessor works in mysterious ways...
#define STR_(x) #x
#define STR(x) STR_(x)
#define VERSION_STRING STR(MAJOR_VERSION) "." STR(MINOR_VERSION_1) "." STR(MINOR_VERSION_2)

// If you have a bumper connected to pin8, uncomment the line below. Remember to cut the brake connection on your motor shield
//#define __Bumper__

// If you have a lift sensor connection to front wheel (connected to pin9), uncomment this line
//#define __Lift_Sensor__

// Do you have a Sensor? If so, uncomment one of these lines
//#define __MS5883L__ true
//#define __MS9150__ true

// Do you have a Display? If so, uncomment one of these lines
//#define __OLED__
//#define __LCD__
#define LCD_I2C_ADDRESS 0x3F

// Do you have a clock? If so, uncomment the line below
//#define __RTC_CLOCK__
#define GO_OUT_TIME         16, 00
#define GO_HOME_TIME        22, 00

// Tiltangle
#define TILTANGLE           45

// Flipangle (turn off cutter and stop everything)
#define FLIPANGLE           75

// Motor Speeds
#define FULLSPEED           100
#define SLOWSPEED           30
#define CUTTERSPEED           100
#define ACCELERATION_DURATION 400

// Enable this if you need the mower to go backward until it's inside and then turn.
// Default behavior is to turn directly when mower is outside BWF, if definition below is enabled this might help mower not to get stuck in slopes.
// If mower is not inside within x seconds mower will stop.
//#define GO_BACKWARD_UNTIL_INSIDE
//#define MAX_GO_BACKWARD_TIME  5 // try to get inside for max x seconds, then stop and error.

class DEFINITION {
  public:
    void definePinsInputOutput();
    void setDefaultLevels(BATTERY* battery, WHEELMOTOR* left, WHEELMOTOR* right, CUTTERMOTOR* cutter);

  private:
};


// Include LocalDefinition.h if it exists
#if __has_include("LocalDefinition.h")
#include "LocalDefinition.h"
#endif

#endif /* _DEFINITION_H_ */
