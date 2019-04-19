#ifndef CAM_ORIG
#define CAM_ORIG

#include <Arduino.h>

#include "AdvMath.h"
#include "Angle.h"
#include "Dist.h"

enum RotPos {
	LEFT = -1, CENTER, RIGHT
};
enum DiffPos {
	NONE, SMALL, LARGE
};
#include "Debug.h"
#include "AdvSerial.h"

typedef struct {
	Dist distGK; // 0~2
	int8_t posOwn;
	RotPos rotOwn;
	DiffPos diffOwn;
	Dist distFW; // 0~2
	Angle rotOpp;
	bool isWide; // 0~1
	bool isInCorner; // 0~1
} cam_t;

class Cam {
	public:
		Cam(uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
		cam_t get();
		int16_t multiRotGoal(Angle);
		bool getCanUse();
		void send(double);
	private:
		AdvSerial sCam;

		uint8_t P_ONOFF;

		uint8_t CENTER_OWN_GOAL;

		cam_t goal;

		uint16_t SLOPE_RG;
		uint16_t INTERCEPT_RG;
};

#endif