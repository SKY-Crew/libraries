#ifndef LINE_ORIG
#define LINE_ORIG

#include <Arduino.h>

#include "AdvMath.h"
#include "Angle.h"
#include "Debug.h"

typedef struct {
	bool isOutside;
	bool isWholeOut;
	bool isHalfOut;
	bool isInAir;
	Angle dirInside;
	bool canPause;
	bool isFront;
	bool isBack;
	double insideRate;
} line_t;

enum Color {
	BLACK, WHITE, GREEN
};

class Line {
	public:
		Line(bool, uint8_t, uint8_t*, uint8_t,
			uint16_t, uint16_t, uint8_t, double);
		line_t get(Angle, Angle);
		line_t modify(bool, Angle, bool, bool);
		uint16_t *getVal();
		Color *getState();
		uint8_t getQTY();
	private:
		bool CAN_LEAVE_LINE;

		uint8_t QTY;
		uint8_t *PORT;

		uint8_t MAX_CIIA;
		uint16_t THRE_BLACK;
		uint16_t THRE_WHITE;
		uint8_t THRE_IS_IN_AIR;

		double CHANGE_RATE;

		uint16_t *val;
		uint16_t *crt;
		Color *state;
		Color *crtState;
		Angle prvDI = false;

		uint8_t countIIA = 0;

		line_t line;
};

#endif