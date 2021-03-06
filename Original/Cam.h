#ifndef CAM_ORIG
#define CAM_ORIG

#include <Arduino.h>

#include "AdvMath.h"
#include "Angle.h"
#include "Dist.h"
#include "PlD.h"
#include "Debug.h"
#include "AdvSerial.h"

typedef struct {
	Angle rotOpp;
	bool isOppWide;

	Angle rotOwn;
	double distOwn;
	Side sideOwn;
	Diff diffOwn;

	Side isInCorner;
} cam_t;

class Cam {
	public:
		Cam(uint8_t, uint8_t, uint8_t, double*, double*, double);
		cam_t get(bool);
		int16_t calRot(Angle);
		bool getCanUse();
		void send(double, bool);
	private:
		AdvSerial sCam;

		uint8_t P_ONOFF;

		cam_t goal;

		Angle crtRot;
		double crtDist;

		PlD pld;
};

#endif