// MAX116xx ADC library

#include "MAX116xx.h"

#define BUFFERSIZE(x) sizeof(x)/sizeof(x[0])

MAX116xx::MAX116xx(int address, int scl, int sda) {
  mAddress = address;
  setAdcCount(address, false);
}

uint8_t MAX116xx::setAdcCount(uint8_t chipType, bool externalRef) {
  //Set adc count according to chip type
  switch (chipType) {
    case MAX11606_ADDRESS: //Or MAX11607
      mAdcCount = externalRef ? 3 : 4;
      break;
    case MAX11608_ADDRESS: //Or MAXll609
      mAdcCount = 8;  //On 11608 and 11609, External REF pin is not shared with an ADC
      break;
    case MAX11610_ADDRESS: //Or MAX11611
      mAdcCount = externalRef ? 11 : 12;
      break;
    default: //If incorrect or not found, set to lowest possible value
      mAdcCount = 3;
      break;
  }
  return mAdcCount;
}

void MAX116xx::begin() {
  Wire.begin();
}


uint16_t MAX116xx::readADC(uint8_t channel) {
  if(channel >= mAdcCount) return 0;

  //Write the config to read a single channel
  writeConfig(SCANMODE_SINGLE, channel, ADCMODE_SINGLE_ENDED);

  Wire.requestFrom(mAddress, 2);
  uint8_t data[2];
  uint8_t err = Wire.readBytes(data, sizeof(data) / sizeof(data[0]));
  if (err != 0) {
    uint16_t result = data[0];
    return ((result << 8) | data[1]) & 0x03ff;
  }

  return 0;
}

uint16_t MAX116xx::readAll(uint8_t *buf){
  writeConfig(SCANMODE_UPTO, mAdcCount, ADCMODE_SINGLE_ENDED);

  Wire.requestFrom(mAddress, 2*mAdcCount);
  uint8_t err = Wire.readBytes(buf, 2*mAdcCount);

  return 0;
}

uint8_t MAX116xx::writeSetup(uint8_t setup) {
  Wire.beginTransmission(mAddress);

  Wire.write(setup | 0x80); //For setup, first bit is always 1

  return Wire.endTransmission();
}

uint8_t MAX116xx::writeConfig(uint8_t config) {
  Wire.beginTransmission(mAddress);

  Wire.write(config & 0x7f); //For config, first bit is always 0

  return Wire.endTransmission();
}

uint8_t MAX116xx::writeSetup(uint8_t reference, uint8_t clockSource, uint8_t bipUni, uint8_t resetConfig) {
  uint8_t setupByte = 0x80 // Start with MSB=1 for setup byte
    | (reference & 0x07) << 4     //Reference select are bits 6:4
    | (clockSource & 0x01) << 3   //clock source bit is bit 3
    | (bipUni & 0x01) << 2         //bipolar/unipolar set is bit 2
    | (resetConfig & 0x01) << 1;    //reset or keep config is bit 1
  //Bit 0 is unused in setup

  //Update adc count when writing setup register
  setAdcCount(mAddress, reference==REFERENCE_EXTERNAL);

  Wire.beginTransmission(mAddress);
  Wire.write(setupByte);
  return Wire.endTransmission();

}

uint8_t MAX116xx::writeConfig(uint8_t scanMode, uint8_t channel, uint8_t singleDif) {
  uint8_t configByte = 0x00 // MSB 0 for configByte
    | (scanMode & 0x03) << 5 //Scanmode bits are bits 5 & 6
    | (channel & 0x0F) << 1  //channel bits are bits 4:1
    | (singleDif & 0x01);     //Single/Differential bit is bit 0

  Wire.beginTransmission(mAddress);
  Wire.write(configByte);
  return Wire.endTransmission();
}

uint8_t MAX116xx::getAdcCount() {
    return mAdcCount;
}



// uint8_t MAX116xx::readConfig() {
//   uint8_t data;
//   uint8_t err = readRegister(MAX116xx_CONFIG, &data);
//   if(err==0) return data;
//   return 0;
// }

// void MAX116xx::writeThresholdHigh(uint16_t threshold) {
//   writeRegister(MAX116xx_THRESHOLD_HIGH, threshold >> 8);
//   writeRegister(MAX116xx_THRESHOLD_LOW, threshold & 0xff);
// }
// uint8_t MAX116xx::writeRegister(uint8_t reg, uint8_t value) {
//   Wire.beginTransmission(mAddress);
//   Wire.write(reg);
//   Wire.write(value);
//   return Wire.endTransmission();
// }

// uint8_t MAX116xx::readRegister(uint8_t reg, uint8_t *data) {
//   Wire.requestFrom(mAddress, 2);
//   *data = (uint8_t)Wire.read();
//   return Wire.endTransmission();
// }

// uint8_t MAX116xx::readRegister(uint8_t reg, uint8_t *data, uint8_t len) {
//   Wire.beginTransmission(mAddress);
//   Wire.write(reg);
//   for (uint8_t i = 0; i < len; i++) {
//     data[i] = Wire.read();
//   }
//   return Wire.endTransmission();
// }