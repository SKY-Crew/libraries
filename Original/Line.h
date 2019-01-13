#ifndef LINE_ORIG
#define LINE_ORIG

#include "Arduino.h"
#include "AdvMath.h"
#include "Angle.h"

typedef struct {
	bool isOutside;
	bool isWholeOut;
	bool isHalfOut;
	bool isInAir;
	Angle dirInside;
	bool canPause;
} line_t;

class Line {
	public:
		Line(bool, uint8_t, uint8_t*, uint8_t,
			uint16_t, uint16_t, uint8_t, double);
		line_t get(bool, bool, Angle);
		uint16_t *getValue();
		uint8_t *getState();
		uint8_t getQTY();
	private:
		bool CAN_LEAVE_LINE;

		uint8_t QTY;
		uint8_t *PORT;

		uint8_t MAX_CIIA;
		uint16_t BORDER_BLACK;
		uint16_t BORDER_WHITE;
		uint8_t BORDER_IS_IN_AIR;

		double MULTI_AVG;

		uint16_t *value;
		uint16_t *crt;
		uint8_t *state;
		uint8_t *crtState;
		Angle prvDI = false;

		uint8_t countIIA = 0;

		line_t line;
};

#endif