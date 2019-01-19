#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"
#include <float.h>
#include "math.h"

MPU6050 _gyro_mpu(0x68);
int _gyro_base = 0;
int _gyro_oldPinState;
int16_t toSend;

void _gyro_initialize() {
  _gyro_mpu.initialize();
  _gyro_mpu.dmpInitialize();
  _gyro_mpu.setDMPEnabled(true);
}

void gyroSetup() {
  CAMERA_I2C_WIRE.begin();
  CAMERA_I2C_WIRE.setClock(400000);

  pinMode(MISO, OUTPUT);

  _gyro_initialize();

  pinMode(GYRO_RESET_PIN, INPUT);
  _gyro_oldPinState = digitalRead(GYRO_RESET_PIN);

  pinMode(GYRO_ONOFF_PIN, INPUT);
}

float _gyro_calcYaw(uint8_t *fifoBuf) {
  Quaternion q;
  VectorFloat gravity;
  float ypr[3];

  _gyro_mpu.dmpGetQuaternion(&q, fifoBuf);
  _gyro_mpu.dmpGetGravity(&gravity, &q);
  _gyro_mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
  return ypr[0];
}

Angle getGyro() {
  if(!digitalRead(GYRO_ONOFF_PIN)) {
    int fifoSize;
    const int packetSize = 42;
    uint8_t fifoBuf[64];
    float yaw;
    int16_t rounded;
    int newPinState;

    fifoSize = _gyro_mpu.getFIFOCount();
    while(fifoSize > 768) {
      _gyro_mpu.resetFIFO();
      fifoSize = _gyro_mpu.getFIFOCount();
    }

    if(fifoSize < 2 * packetSize) {// while(fifoSize < 2 * packetSize) {
      delay(1);
      return toSend;//   fifoSize = _gyro_mpu.getFIFOCount();
    }

    _gyro_mpu.getFIFOBytes(fifoBuf, packetSize);
    fifoSize -= packetSize;

    yaw = _gyro_calcYaw(fifoBuf) * M_PI * -18.2369953125F;
    newPinState = digitalRead(GYRO_RESET_PIN);

    rounded = (int16_t)round(yaw);
    toSend = rounded - _gyro_base;

    if (newPinState == HIGH && _gyro_oldPinState == LOW) _gyro_base = rounded;

    _gyro_oldPinState = newPinState;

    return Angle(toSend);
  }else {
    delay(1);
    return false;
  }
}

int16_t multiRotGyro(Angle rotGyro) {
  uint8_t key = 0;
  for(; key < SIZE_SLOPE_RG - 1; key ++) {
    if(absAngle(rotGyro) <= POINT_RG[key]) { break; }
  }
  return signum(rotGyro) * SLOPE_RG[key];
}

bool getCanUseGyro() {
  return !digitalRead(GYRO_ONOFF_PIN);
}
