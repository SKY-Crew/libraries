#include "LCD.h"

LCD::LCD(uint8_t given_P_REDRAW, uint8_t given_P_SCK, uint8_t given_P_MOSI, uint8_t given_P_CS, uint8_t given_P_DI,
	uint32_t given_WAIT, uint8_t given_MAX_CP, uint8_t given_QTY_LINE) {
	//copy
	P_REDRAW = given_P_REDRAW;
	MAX_CP = given_MAX_CP;
	lcd = new MGLCD_AQM1248A_SoftwareSPI(MGLCD_SpiPin4(given_P_SCK, given_P_MOSI, given_P_CS, given_P_DI), given_WAIT);

	QTY_LINE = given_QTY_LINE;
	SIN_LINE = new double[QTY_LINE];
	COS_LINE = new double[QTY_LINE];

	for(uint8_t numLine = 0; numLine < QTY_LINE; numLine ++) {
		Angle angleLine = numLine * 360.0 / QTY_LINE + 90;
		SIN_LINE[numLine] = sin(angleLine);
		COS_LINE[numLine] = cos(angleLine);
	}

	//init
	pinMode(P_REDRAW, INPUT);

	lcd->Reset();
	if(strlen("ｱ")!=1) {
		lcd->SetCodeMode(MGLCD_CODE_UTF8);
	}
	lcd->SetRegulatorVoltage(3);
	lcd->SetVolumeResistor(28);
	delay(500);
	lcd->print("Start!");
}

void LCD::clear(bool resetPage) {
	lcd->ClearScreen();
	if(resetPage) {
		page = 0;
	}
}

void LCD::write(String data, uint8_t XCoord, uint8_t YCoord) {
	lcd->Locate(XCoord, YCoord);
	lcd->print(data);
}

void LCD::drawMeter(Angle theta, uint8_t XCoord, bool canDrawCircle) {
	theta += 180;
	if(theta.inside(90, 270)) {
		lcd->Locate(3 + XCoord, 2);
	}else {
		lcd->Locate(3 + XCoord, 3);
	}
	lcd->print(string(theta));
	if(canDrawCircle) {
		lcd->Circle(24 + XCoord * 6, 24, 23);
	}
	double lineX = sin(theta) * 23;
	double lineY = -cos(theta) * 23;
	double lineWeightX = -cos(theta) * 0.5;
	double lineWeightY = -sin(theta) * 0.5;
	for(int8_t numLine = -1; numLine <= 1; numLine ++) {
		lcd->Line(	24 + lineWeightX * numLine + XCoord * 6,
					24 + lineWeightY * numLine,
					24 + lineWeightX * numLine + lineX + XCoord * 6,
					24 + lineWeightY * numLine + lineY);
	}
}

void LCD::drawAngelRing(uint8_t QTY_LINE, Color *stateLine, uint8_t XCoord) {
	for(uint8_t numLine = 0; numLine < QTY_LINE; numLine ++) {
		uint8_t XCoordLine = 24 + 20 * COS_LINE[numLine] + XCoord * 6;
		uint8_t YCoordLine = 24 + 20 * SIN_LINE[numLine];

		if(stateLine[numLine] == BLACK) {
			lcd->FillCircle(XCoordLine, YCoordLine, 1);
		}else if(stateLine[numLine] == WHITE) {
			lcd->Circle(XCoordLine, YCoordLine, 2);
		}else {
			lcd->FillCircle(XCoordLine, YCoordLine, 2);
		}
	}
}

void LCD::redraw() {
	if(digitalRead(P_REDRAW)) {
		while(digitalRead(P_REDRAW)) {  }
		page = (page + 1) % MAX_PAGE;
	}
}