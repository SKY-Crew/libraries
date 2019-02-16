#ifndef GYRO_ORIG
#define GYRO_ORIG

#include "MPU6050.h"
#include "I2Cdev.h"
#include <float.h>
#include "math.h"
#include "AdvMath.h"
#include "Angle.h"

class Gyro {
  public:
    Gyro(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, int16_t*, int16_t*);
    Angle get();
    int16_t multiRot(Angle);
    bool getCanUse();
  private:
    void _gyro_initialize();
    float _gyro_calcYaw(uint8_t*);
    MPU6050 *_gyro_mpu;
    int _gyro_base = 0;
    int _gyro_oldPinState;
    int16_t toSend;

   TwoWire *WireX;

    uint8_t ONOFF_PIN;
    uint8_t RESET_PIN;
    uint8_t SIZE_SLOPE_RG;
    int16_t *SLOPE_RG;
    int16_t *POINT_RG;
};

#endif