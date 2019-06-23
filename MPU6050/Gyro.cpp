#include "Gyro.h"

void Gyro::_gyro_initialize() {
  _gyro_mpu->initialize();
  _gyro_mpu->dmpInitialize();
  _gyro_mpu->setDMPEnabled(true);
}

Gyro::Gyro(uint8_t P_WIRE, uint8_t PORT, uint8_t ONOFF_PIN, uint8_t RESET_PIN,
  uint8_t SIZE_POINT, double *POINT, double *ROT, double Kd,
  uint8_t BROKEN_THRE, uint8_t STOP_FRAMES, uint8_t STAY_THRE) {
  // copy
  wGyro.set(P_WIRE);

  this->ONOFF_PIN = ONOFF_PIN;
  this->RESET_PIN = RESET_PIN;

  pld.set(SIZE_POINT, POINT, ROT, Kd);

  this->BROKEN_THRE = BROKEN_THRE;

  // init
  _gyro_mpu = new MPU6050(PORT);
  wGyro.get()->begin();
  wGyro.get()->setClock(4*1000*1000);

  pinMode(MISO, OUTPUT);

  _gyro_initialize();

  pinMode(RESET_PIN, INPUT);
  _gyro_oldPinState = digitalRead(RESET_PIN);

  pinMode(ONOFF_PIN, INPUT);

  stayCounter.set_MAX(STAY_THRE);
  // brokenCounter.set_MAX(STOP_FRAMES);
  // brokenCounter.set_COUNT_UP(false);
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
  if(/*!bool(brokenCounter) && */!digitalRead(ONOFF_PIN)) {
    int fifoSize;
    const int packetSize = 42;
    uint8_t fifoBuf[64];
    float yaw;
    int16_t rounded;
    int newPinState;

    fifoSize = _gyro_mpu->getFIFOCount();
    while(fifoSize > 768) {
      trace(30) { Serial.println("too much data"); }
      _gyro_mpu->resetFIFO();
      fifoSize = _gyro_mpu->getFIFOCount();
    }
    trace(30) { Serial.println("fifosize: " + str(fifoSize)); }

    while (fifoSize % 42 != 0) {
      trace(30) { Serial.println("data uncompleted ///// reset ////"); }
      int64_t from = micros();
      _gyro_mpu->resetFIFO();
      int64_t to = micros();
      trace(30) { Serial.println("reset finished. took " + str(to - from) + "μs"); }
      fifoSize = _gyro_mpu->getFIFOCount();
    }
    if(fifoSize < 2 * packetSize) {
      trace(30) { Serial.println("no data"); }
      return crt;
    }

    _gyro_mpu->getFIFOBytes(fifoBuf, packetSize);
    trace(30) { Serial.println("fifosize AFTER: " + str(_gyro_mpu->getFIFOCount())); }

    yaw = _gyro_calcYaw(fifoBuf) * M_PI * -18.2369953125F;
    newPinState = digitalRead(RESET_PIN);

    rounded = (int16_t)round(yaw);

    bool baseChanged = false;
    if (newPinState == HIGH && _gyro_oldPinState == LOW) {
      baseChanged = true;
      _gyro_base = rounded;
    }

    toSend = rounded - _gyro_base;

    _gyro_oldPinState = newPinState;

    crt = toSend;
    // brokenCounter.increase(bool(crt) && bool(prv) && (!baseChanged)
    //                        && abs(crt - prv) > BROKEN_THRE);

    trace(5) { Serial.println("Gyro:"+str(crt)); }

    if (bool(crt) && bool(prv) && (!baseChanged)
        && abs(crt - prv) > BROKEN_THRE) {
      trace(30) { Serial.println("######## broken moving ########"); }
      _gyro_mpu->resetFIFO();
      return crt = prv;
    }
    stayCounter.increase(prv == crt);
    if (bool(stayCounter)) {
      trace(30) { Serial.println("-------- reset FIFO --------"); }
      _gyro_mpu->resetFIFO();
      stayCounter.reset();
    }
    return crt;
  }else {
    // brokenCounter.increase(false);
    return crt = false;
  }
}

Angle Gyro::getDiff() {
  return crt - prv;
}

int16_t Gyro::calRot(Angle origin) {
  return pld.cal(crt, origin, digitalRead(RESET_PIN));
}

bool Gyro::getCanUse() {
  return !digitalRead(ONOFF_PIN);
}