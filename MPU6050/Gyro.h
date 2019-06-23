#ifndef GYRO_ORIG
#define GYRO_ORIG

#include "MPU6050.h"
#include "I2Cdev.h"
#include <float.h>
#include "math.h"

#include "AdvMath.h"
#include "Angle.h"
#include "Count.h"
#include "PlD.h"
#include "Debug.h"
#include "AdvWire.h"

class Gyro {
  public:
    Gyro(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, double*, double*, double, uint8_t, uint8_t, uint8_t);
    Angle get();
    Angle getDiff();
    int16_t calRot(Angle);
    bool getCanUse();
  private:
    void _gyro_initialize();
    float _gyro_calcYaw(uint8_t*);
    MPU6050 *_gyro_mpu;
    int _gyro_base = 0;
    int _gyro_oldPinState;
    int16_t toSend;

    AdvWire wGyro;

    Angle crt;
    Angle prv;

    uint8_t ONOFF_PIN;
    uint8_t RESET_PIN;

    PlD pld;

    Count stayCounter;
    // Count brokenCounter;
    uint8_t BROKEN_THRE;
};

#endif