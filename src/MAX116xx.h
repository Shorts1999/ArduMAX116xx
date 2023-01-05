#ifndef __MAX116xx_H
#define __MAX116xx_H

#include <Arduino.h>
#include <Wire.h>

#define MAX11606_ADDRESS 0x34
#define MAX11607_ADDRESS 0x34
#define MAX11608_ADDRESS 0x33
#define MAX11609_ADDRESS 0x33
#define MAX11610_ADDRESS 0x35
#define MAX11611_ADDRESS 0x35

// Register addresses
#define MAX116xx_CONVERSION 0x00
#define MAX116xx_CONFIG 0x01
#define MAX116xx_THRESHOLD_HIGH 0x02
#define MAX116xx_THRESHOLD_LOW 0x03
#define MAX116xx_STATUS 0x04

class MAX116xx {
public:
  MAX116xx(int address, int scl = SCL, int sda = SDA);
  // MAX116xx(int address, TwoWire *twoWire);
  void begin();
  uint16_t readADC(uint8_t channel);
  uint8_t writeConfig(uint8_t config);
  uint8_t readConfig();
  void writeThresholdHigh(uint16_t threshold);
  uint16_t readThresholdHigh();
  void writeThresholdLow(uint16_t threshold);
  uint16_t readThresholdLow();
  uint8_t readStatus();

private:
  int mAddress;
  uint8_t readRegister(uint8_t reg, uint8_t *data);
  uint8_t readRegister(uint8_t reg, uint8_t *data, uint8_t len);
  uint8_t writeRegister(uint8_t reg, uint8_t value);
};

#endif //_MAX116xx_H