/*
 Liam - DIY Robot Lawn Mower

 BWF Sensor Library

 ======================
  Licensed under GPLv3
 ======================
*/
/*
  The BWF sensor works by measuring the time between rising edges

  Example:
    Signal in BWF

    I
    ^
    |      _         _
    |     | |       | |
    |     | |       | |
    |...__| |  _____| |  ___...
    |       | |       | |
    |       | |       | |
    |       |_|       |_|
    |
    +----------------------> t
           1 1   5   1 1    5...

    Outside of the fence, the sensor coil and amplifier circuit will sense the
    rising edges in the signal. Inside the fence, the signal is inverted, and
    the circuit will sense the falling edges of the signal instead.

    In this example, the time between rising edges is 2 units, followed by 5,
    2, 5, and so on. The time between falling edges is 7 units.

  When a rising edge is detected on the currently selected sensor, the function
  readSensor() is run. By keeping track of the last time it was run, it can
  calculate the time between pulses and check if it matches what was expected
  for being inside or outside of the fence.
*/

#include "BWFSensor.h"

int BWFSENSOR::outside_code[] = {OUTSIDE_BWF, INSIDE_BWF-OUTSIDE_BWF};
int BWFSENSOR::inside_code[] = {INSIDE_BWF};

int currentSensor = 0;

BWFSENSOR::BWFSENSOR(int selA, int selB) {
  selpin_A = selA;
  selpin_B = selB;
}


// Select active sensor
void BWFSENSOR::select(int sensornumber) {
  if (currentSensor == sensornumber) {

    return;
  }
  currentSensor = sensornumber;

  digitalWrite(selpin_A, (sensornumber & 1) > 0 ? HIGH : LOW);
  digitalWrite(selpin_B, (sensornumber & 2) > 0 ? HIGH : LOW);
  clearSignal();
  long time = millis();

  while(!gotSignal()
        && millis() - time < BWF_COLLECT_SIGNAL_TIME)
  {
    delay(1);
  }
}


void BWFSENSOR::clearSignal() {
  for (int i = 0; i < arr_length; i++)
    arr[i] = 0;
  arr_count = 0;
}


bool BWFSENSOR::isInside() {
  return matchCode(inside_code, sizeof(inside_code)/sizeof(inside_code[0]));
}

bool BWFSENSOR::isOutside() {
  return matchCode(outside_code, sizeof(outside_code)/sizeof(outside_code[0]));
}

bool BWFSENSOR::isOutOfBounds() {
  if (BWF_DETECTION_ALWAYS)
    return !isInside();
  else
    return isOutside();
}


bool BWFSENSOR::matchCode(int *code, int len)
{
  // Check for code starting at each point of the buffer
  for (int i=0; i < arr_length; i++)
  {
    // Start from the latest entry, going backwards
    int start = (arr_count + arr_length - i) % arr_length;
    bool match = true;
    for (int j = 0; j < len; j++)
    {
      if (abs(arr[start+j] - code[j]) > BWF_CODE_ERROR)
      {
        match = false;
        break;
      }
    }
    if(match)
      return true;
  }

  return false;
}


// This function is run each time the BWF pin gets a pulse
// For accuracy, this function should be kept as fast as possible
void BWFSENSOR::readSensor() {
  long now = micros();

  // Calculate the time since last pulse
  int time_since_pulse = int(now - last_pulse);
  last_pulse = now;

  // Convert to pulse units (rounding up)
  int pulse_length = (time_since_pulse + (pulse_unit_length/2)) / pulse_unit_length;

  // Store the received code
  arr[arr_count++] = pulse_length;
  if (arr_count > arr_length) arr_count=0;
}

void BWFSENSOR::printSignal() {
  for (int i = 0; i < arr_length; i++) {
    Serial.print(arr[i]);
    Serial.print(" ");
  }
}
bool BWFSENSOR::gotSignal()
{
  return arr_count >= BWF_NUMBER_OF_PULSES ? true : false;
}
