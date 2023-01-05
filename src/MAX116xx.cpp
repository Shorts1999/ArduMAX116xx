// MAX116xx ADC library

#include "MAX116xx.h"

#define BUFFERSIZE(x) sizeof(x)/sizeof(x[0])

MAX116xx::MAX116xx(int address, int scl, int sda) {
  mAddress = address;
}

void MAX116xx::begin() {
  Wire.begin();
}

uint8_t MAX116xx::writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(mAddress);
  Wire.write(reg);
  Wire.write(value);
  return Wire.endTransmission();
}

uint8_t MAX116xx::readRegister(uint8_t reg, uint8_t *data) {
  Wire.requestFrom(mAddress, 2);
  *data = (uint8_t)Wire.read();
  return Wire.endTransmission();
}

uint8_t MAX116xx::readRegister(uint8_t reg, uint8_t *data, uint8_t len) {
  Wire.beginTransmission(mAddress);
  Wire.write(reg);
  for (uint8_t i = 0; i < len; i++) {
    data[i] = Wire.read();
  }
  return Wire.endTransmission();
}

uint16_t MAX116xx::readADC(uint8_t channel) {
  Wire.requestFrom(mAddress, 2);

  // Read the conversion result
  uint8_t data[2];
  uint8_t err = Wire.readBytes(data, sizeof(data)/sizeof(data[0]));
  if (err != 0) {
    uint16_t result = data[0];
    return ((result << 8) | data[1]) & 0x03ff;
  }

  return 0;
}

uint8_t MAX116xx::readStatus(){
  return 1;
}

uint8_t MAX116xx::writeConfig(uint8_t config) {
  Wire.beginTransmission(mAddress);

  Wire.write(config);

  return Wire.endTransmission();
}

uint8_t MAX116xx::readConfig() {
  uint8_t data;
  uint8_t err = readRegister(MAX116xx_CONFIG, &data);
  if(err==0) return data;
  return 0;
}

void MAX116xx::writeThresholdHigh(uint16_t threshold) {
  writeRegister(MAX116xx_THRESHOLD_HIGH, threshold >> 8);
  writeRegister(MAX116xx_THRESHOLD_LOW, threshold & 0xff);
}