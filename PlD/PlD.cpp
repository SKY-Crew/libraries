#include "PlD.h"

PlD::PlD() {  }

void PlD::set(uint8_t SIZE_POINT, double *POINT, double *ROT, double Kd) {
	// copy
	this->SIZE_POINT = SIZE_POINT;
	this->POINT = new double[SIZE_POINT];
	copyArray(this->POINT, POINT, SIZE_POINT);
	this->ROT = new double[SIZE_POINT];
	copyArray(this->ROT, ROT, SIZE_POINT);
	this->Kd = Kd;

	// init
}

int16_t PlD::cal(Angle crt, Angle origin, bool reset) {
	error[0] = absMinus(double(crt + origin), 1.5);
	error[1] = reset ? 0 : filter(double(crt - prv) * 10, error[1], 0.6);
	error[1] = absMinus(error[1], 1);
	prv = crt;
	return polyLine(error[0], POINT, ROT, SIZE_POINT) + error[1] * Kd;
}