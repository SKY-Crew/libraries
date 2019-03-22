#include "LCD.h"

void LCD::run(Angle gyro, line_t line, bool canUseKicker, bool canUseCam, bool canUseGyro, bool isFW, bool canUseComc, comc_t fellow,
		uint8_t QTY_LINE, uint16_t *valueLine, Color *stateLine, double valueIna219, cam_t goal,
		vectorRT_t ball, uint8_t QTY_IR, uint16_t *valueIR,
		uint16_t valueCatch, bool catchingBall, uint16_t valueBF, bool isBallForward, Dist distBall,
		uint16_t valueFrontPSD, bool enemyStandsFront, uint16_t valueBackPSD, Dist distGoalPSD) {
	redraw();
	countPrint ++;
	if(countPrint >= MAX_CP) {
		clear(false);
		switch (page) {
			case 0:
				drawMeter(gyro, 0, true);
				drawMeter(line.dirInside, 8, false);
				drawAngelRing(QTY_LINE, stateLine, 8);
				write(canUseKicker ? "kON" : "kOFF", 17, 0);
				write(canUseCam ? "cON" : "cOFF", 17, 1);
				write(canUseGyro ? "gON" : "gOFF", 17, 2);
				write(canUseComc ? "mON" : "mOFF", 17, 3);
				write(fellow.exists ? fellow.isFW ? String(OOO)+"FW" : String(OOO)+"GK" : "---", 17, 4);
				write(isFW ? "FW" : "GK", 17, 5);
				break;
			case 1:
				for(uint8_t numLine = 0; numLine < QTY_LINE; numLine ++) {
					write(valueLine[numLine], (numLine % 5) * 4, floor(numLine / 5.0));
				}
				write("Volt:"+String(valueIna219), 11, 3);
				write("OWN Rot"+String(goal.rot), 0, 4);
				write("Dis"+String(goal.distGK)+","+String(goal.distFW), 10, 4);
				write("Crn"+String(goal.isInCorner ? OOO : '-'), 17, 4);
				write("OPP Rot:"+String(goal.rotOpp), 0, 5);
				write("Wide:"+String(goal.isWide ? OOO : '-'), 11, 5);
				break;
			case 2:
				drawMeter(ball.t, 0, true);
				for(uint8_t numIR = 0; numIR < QTY_IR; numIR ++) {
					write(valueIR[numIR], 9 + (numIR % 3) * 4, floor(numIR / 3.0));
				}
				break;
			case 3:
				write("Catch:"+String(valueCatch), 0, 0);
				write("T/F:"+String(catchingBall ? OOO : '-'), 11, 0);
				write("frwdIR:"+String(valueBF), 0, 1);
				write("T/F:"+String(isBallForward ? OOO : '-'), 11, 1);
				write("Dis:"+String(round(ball.r)), 0, 2);
				write("Close:"+String(distBall == CLOSE ? 'C' : distBall == PROPER ? 'M' : 'F'), 11, 2);
				write("Dis:", 0, 3);
				for(int8_t i = -1; i <= 1; i ++) {
					lcd->Line(30, 9 * 3 + i, 30 + ball.r * 0.15, 9 * 3 + i);
				}
				write("F_PSD:"+String(valueFrontPSD), 0, 4);
				write("C/F:"+String(enemyStandsFront ? OOO : '-'), 11, 4);
				write("B_PSD:"+String(valueBackPSD), 0, 5);
				write("C/F:"+String(distBall == CLOSE ? 'C' : distBall == PROPER ? 'M' : 'F'), 11, 5);
				break;
		}
		MAX_PAGE = 4;
		countPrint = 0;
	}
}

void LCD::run4IR(uint8_t QTY, uint16_t *maxIR, uint16_t *minIR, double *avgIR) {
	redraw();
	countPrint ++;
	if(countPrint >= MAX_CP) {
		clear(false);
		switch (page) {
			case 0:
				write("MAX", 0, 0);
				for(int i = 0; i < QTY; i ++) {
					write(maxIR[i], (i % 4) * 5, 1 + floor(i / 4.0));
				}
				break;
			case 1:
				write("MIN", 0, 0);
				for(int i = 0; i < QTY; i ++) {
					write(minIR[i], (i % 4) * 5, 1 + floor(i / 4.0));
				}
				break;
			case 2:
				write("AVG", 0, 0);
				for(int i = 0; i < QTY; i ++) {
					write((int) avgIR[i], (i % 4) * 5, 1 + floor(i / 4.0));
				}
				break;
		}
		MAX_PAGE = 3;
		countPrint = 0;
	}
}