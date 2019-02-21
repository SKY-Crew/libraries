#include "Gyro.h"

void Gyro::_gyro_initialize() {
  _gyro_mpu->initialize();
  _gyro_mpu->dmpInitialize();
  _gyro_mpu->setDMPEnabled(true);
  _gyro_mpu->initialize();
  _gyro_mpu->dmpInitialize();
  _gyro_mpu->setDMPEnabled(true);
}

Gyro::Gyro(uint8_t given_X, uint8_t given_PORT, uint8_t given_ONOFF_PIN, uint8_t given_RESET_PIN,
  uint8_t given_SIZE_SLOPE_RG, int16_t *given_SLOPE_RG, int16_t *given_POINT_RG) {
  //copy
  wGyro.set(given_X);

  _gyro_mpu = new MPU6050(given_PORT);

  ONOFF_PIN = given_ONOFF_PIN;
  RESET_PIN = given_RESET_PIN;
  SIZE_SLOPE_RG = given_SIZE_SLOPE_RG;
  SLOPE_RG = new int16_t[SIZE_SLOPE_RG];
  copyArray(SLOPE_RG, given_SLOPE_RG, SIZE_SLOPE_RG);
  POINT_RG = new int16_t[SIZE_SLOPE_RG - 1];
  copyArray(POINT_RG, given_POINT_RG, SIZE_SLOPE_RG - 1);
  
  //init
  wGyro.get()->begin();
  wGyro.get()->setClock(400000);

  pinMode(MISO, OUTPUT);

  _gyro_initialize();

  pinMode(RESET_PIN, INPUT);
  _gyro_oldPinState = digitalRead(RESET_PIN);

  pinMode(ONOFF_PIN, INPUT);
}

float Gyro::_gyro_calcYaw(uint8_t *fifoBuf) {
  Quaternion q;
  VectorFloat gravity;
  float ypr[3];

  _gyro_mpu->dmpGetQuaternion(&q, fifoBuf);
  _gyro_mpu->dmpGetGravity(&gravity, &q);
  _gyro_mpu->dmpGetYawPitchRoll(ypr, &q, &gravity);
  return ypr[0];
}

Angle Gyro::get() {
  if(!digitalRead(ONOFF_PIN)) {
    int fifoSize;
    const int packetSize = 42;
    uint8_t fifoBuf[64];
    float yaw;
    int16_t rounded;
    int newPinState;

    fifoSize = _gyro_mpu->getFIFOCount();
    while(fifoSize > 768) {
      _gyro_mpu->resetFIFO();
      fifoSize = _gyro_mpu->getFIFOCount();
    }

    if(fifoSize < 2 * packetSize) {// while(fifoSize < 2 * packetSize) {
      return toSend;//   fifoSize = _gyro_mpu->getFIFOCount();
    }

    _gyro_mpu->getFIFOBytes(fifoBuf, packetSize);
    fifoSize -= packetSize;

    yaw = _gyro_calcYaw(fifoBuf) * M_PI * -18.2369953125F;
    newPinState = digitalRead(RESET_PIN);

    rounded = (int16_t)round(yaw);
    toSend = rounded - _gyro_base;

    if (newPinState == HIGH && _gyro_oldPinState == LOW) _gyro_base = rounded;

    _gyro_oldPinState = newPinState;

    return Angle(toSend);
  }else {
    return false;
  }
}

int16_t Gyro::multiRot(Angle rotGyro) {
  uint8_t key = 0;
  for(; key < SIZE_SLOPE_RG - 1; key ++) {
    if(absAngle(rotGyro) <= POINT_RG[key]) { break; }
  }
  return signum(rotGyro) * SLOPE_RG[key];
}

bool Gyro::getCanUse() {
  return !digitalRead(ONOFF_PIN);
}