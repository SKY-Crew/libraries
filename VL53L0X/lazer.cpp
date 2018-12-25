#include "lazer.h"

Lazer::Lazer(uint8_t get_X, double get_MULTI_AVG, uint16_t get_BORDER_IS_CLOSE) {
	//copy
	lazer = new VL53L0X(get_X);

	MULTI_AVG = get_MULTI_AVG;
	BORDER_IS_CLOSE = get_BORDER_IS_CLOSE;

	//init
	lazer->init();
	lazer->setTimeout(500);
	//lazer->setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
	//lazer->setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 8);
	lazer->startContinuous();
}

bool Lazer::get() {
	if((lazer->readReg(lazer->RESULT_INTERRUPT_STATUS) & 0x07) != 0) {
		value = lazer->readRangeContinuousMillimeters();
		if(value < 800) {
			value = value * (1 - MULTI_AVG) + prv * MULTI_AVG;
		}else {
			value = 800;
		}
		prv = value;
	}
	return value <= BORDER_IS_CLOSE;
}

double Lazer::getValue() {
	return value;
}