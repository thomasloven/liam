/*
 Liam - DIY Robot Lawn Mower

 Motion Sensor Library

 ======================
  Licensed under GPLv3
 ======================
*/

#ifndef _MOTIONSENSOR_H_
#define _MOTIONSENSOR_H_

#include <Wire.h>  // For Compass
#include <I2Cdev.h>
#include "Definition.h"

class MOTIONSENSOR {
  public:
    virtual boolean initialize();
    virtual void updateHeading();
    virtual int getHeading();
    virtual void setNewTargetHeading();
    virtual int headingVsTarget();
    virtual int getTiltAngle();

  protected:
    int current_heading;
    int target_heading;
    int tilt_angle;
    int16_t mx, gx, ax;
    int16_t my, gy, ay;
    int16_t mz, gz, az;
};

#endif /* _MOTIONSENSOR_H_ */
