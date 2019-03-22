#ifndef CAM_ORIG
#define CAM_ORIG

#include <Arduino.h>
#include "AdvMath.h"
#include "AdvSerial.h"

typedef struct {
	Dist distGK;//0~2
	int8_t rot;//-3~3(0~6)
	int8_t distFW;//0~2
	int8_t rotOpp;//-3~4(0~7)
	bool isWide;//0~1
	uint8_t isInCorner;//0~1
} cam_t;

class Cam {
	public:
		Cam(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t);
		cam_t get();
		int16_t multiRotGoal(int16_t);
		bool getCanUse();
	private:
		HardwareSerial *SerialX;
		AdvSerial sCam;

		uint8_t P_ONOFF;

		uint8_t CENTER_OPP_GOAL;
		uint8_t CENTER_OWN_GOAL;

		cam_t goal;

		uint16_t SLOPE_RG;
		uint16_t INTERCEPT_RG;
};

#endif