// LiamState.h

#ifndef _LIAMSTATE_h
#define _LIAMSTATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


enum SensorState {
	Inactive,
	OutOfBounds,
	InsideBounds,
};

class LiamState {

public:
	LiamState();
	SensorState Sensor0;
	SensorState Sensor1;
};


#endif
