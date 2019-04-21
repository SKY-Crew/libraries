#include "INA219.h"

INA219::INA219(uint8_t P_WIRE, double LOW_VOLT, double HIGH_VOLT, double MAX_VOLT, uint8_t MAX_CR, uint8_t MAX_CVL) {
	//copy
	this->LOW_VOLT = LOW_VOLT;
	this->HIGH_VOLT = HIGH_VOLT;
	this->MAX_VOLT = MAX_VOLT;
	this->MAX_CR = MAX_CR;
	this->MAX_CVL = MAX_CVL;

	//init
	ina219.begin(P_WIRE, 0x41);
}

bool INA219::checkVolt() {
	countRead ++;
	if(countRead >= MAX_CR) {
		val = ina219.getBusVoltage_V();
		if(countVoltLow >= MAX_CVL) {
			countVoltLow = val > HIGH_VOLT ? 0 : MAX_CVL;
			return true;
		}else {
			countVoltLow = val < LOW_VOLT ? (countVoltLow + 1) : 0;
		}
	countRead = 0;
	}
	return false;
}

double INA219::getVal() {
	return val;
}
double INA219::getRate() {
	return map(val, LOW_VOLT, MAX_VOLT, 0, 1);
}