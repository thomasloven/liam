/*
 Liam - DIY Robot Lawn Mower

 BWF Sensor Library

 ======================
  Licensed under GPLv3
 ======================
*/

#include <Arduino.h>
#include "Definition.h"

#ifndef _BWFSENSOR_H_
#define _BWFSENSOR_H_

// BWF Code for timout and no signal (in milliseconds)
#define TIMEOUT_DELAY    20000


class BWFSENSOR {
  public:
    BWFSENSOR(int selA, int selB);

    void select(int sensornumber);
    void clearSignal();

    bool isInside();
    bool isOutside();
    bool isOutOfBounds();
    bool hasNoSignal();
    bool gotSignal();

    bool matchCode(int *code, int len);

    void readSensor();

    void printSignal();

  private:
    // BWF Code for inside and outside the fence
    static int inside_code[];
    static int outside_code[];

    const static int pulse_unit_length = 100;

    int selpin_A;
    int selpin_B;

    long last_pulse;

    // Array for debug printing
    const static int arr_length=10;
    int arr[arr_length];
    int arr_count;
};

#endif /* _BWFSENSOR_H_ */
