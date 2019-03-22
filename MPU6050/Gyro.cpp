#include "Gyro.h"

void Gyro::_gyro_initialize() {
  _gyro_mpu->initialize();
  _gyro_mpu->dmpInitialize();
  _gyro_mpu->setDMPEnabled(true);
  _gyro_mpu->initialize();
  _gyro_mpu->dmpInitialize();
  _gyro_mpu->setDMPEnabled(true);
}

Gyro::Gyro(uint8_t P_WIRE, uint8_t PORT, uint8_t ONOFF_PIN, uint8_t RESET_PIN,
  uint8_t SIZE_SLOPE_RG, int16_t *SLOPE_RG, int16_t *POINT_RG) {
  //copy
  wGyro.set(P_WIRE);

  _gyro_mpu = new MPU6050(PORT);

  this->ONOFF_PIN = ONOFF_PIN;
  this->RESET_PIN = RESET_PIN;
  this->SIZE_SLOPE_RG = SIZE_SLOPE_RG;
  this->SLOPE_RG = new int16_t[SIZE_SLOPE_RG];
  copyArray(this->SLOPE_RG, SLOPE_RG, SIZE_SLOPE_RG);
  this->POINT_RG = new int16_t[SIZE_SLOPE_RG - 1];
  copyArray(this->POINT_RG, POINT_RG, SIZE_SLOPE_RG - 1);
  
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
  prv = crt;
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

    crt = toSend;
    return crt;
  }else {
    crt = false;
    return false;
  }
}

Angle Gyro::getDiff() {
  return crt - prv;
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