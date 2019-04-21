#include "LCD.h"

LCD::LCD(uint8_t P_REDRAW, uint8_t P_SCK, uint8_t P_MOSI, uint8_t P_CS, uint8_t P_DI,
	uint32_t WAIT, uint8_t MAX_CP, uint8_t QTY_LINE) {
	// copy
	this->P_REDRAW = P_REDRAW;
	this->MAX_CP = MAX_CP;
	lcd = new MGLCD_AQM1248A_SoftwareSPI(MGLCD_SpiPin4(P_SCK, P_MOSI, P_CS, P_DI), WAIT);

	this->QTY_LINE = QTY_LINE;
	SIN_LINE = new double[QTY_LINE];
	COS_LINE = new double[QTY_LINE];

	for(uint8_t numLine = 0; numLine < QTY_LINE; numLine ++) {
		Angle angleLine = numLine * 360.0 / QTY_LINE + 90;
		SIN_LINE[numLine] = sin(angleLine);
		COS_LINE[numLine] = cos(angleLine);
	}

	// init
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
	Angle convertedTheta = theta + 180;
	lcd->Locate((theta < 0 && bool(theta) ? 2 : 3) + XCoord, convertedTheta.isDown(90) ? 2 : 3);
	lcd->print(str(theta));
	if(canDrawCircle) {
		lcd->Circle(24 + XCoord * 6, 24, 23);
	}
	if(bool(theta)) {
		double lineX = sin(convertedTheta) * 23;
		double lineY = -cos(convertedTheta) * 23;
		double lineWeightX = -cos(convertedTheta) * 0.5;
		double lineWeightY = -sin(convertedTheta) * 0.5;
		for(int8_t numLine = -1; numLine <= 1; numLine ++) {
			lcd->Line(	24 + lineWeightX * numLine + XCoord * 6,
						24 + lineWeightY * numLine,
						24 + lineWeightX * numLine + lineX + XCoord * 6,
						24 + lineWeightY * numLine + lineY);
		}
	}
}

void LCD::drawAngelRing(uint8_t QTY_LINE, Color *stateLine, uint8_t XCoord) {
	for(uint8_t numLine = 0; numLine < QTY_LINE; numLine ++) {
		uint8_t XCoordLine = 24 + 20 * COS_LINE[numLine] + XCoord * 6;
		uint8_t YCoordLine = 24 + 20 * SIN_LINE[numLine];

		switch(stateLine[numLine]) {
			case BLACK:
				lcd->FillCircle(XCoordLine, YCoordLine, 1);
				break;
			case WHITE:
				lcd->Circle(XCoordLine, YCoordLine, 2);
				break;
			case GREEN:
				lcd->FillCircle(XCoordLine, YCoordLine, 2);
				break;
		}
	}
}

void LCD::redraw() {
	if(digitalRead(P_REDRAW) && !prvRedraw) {
		page = (page + 1) % MAX_PAGE;
	}
	prvRedraw = digitalRead(P_REDRAW);
}