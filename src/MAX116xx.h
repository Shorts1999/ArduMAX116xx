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

class MAX116xx {
public:
  MAX116xx(int address, int scl = SCL, int sda = SDA);
  // MAX116xx(int address, TwoWire *twoWire);
  void begin();
  uint16_t readADC(uint8_t channel);

  uint8_t writeSetup(uint8_t setup);
  uint8_t writeSetup(uint8_t reference, uint8_t clockSource, uint8_t bipUni, uint8_t resetConfig);
  uint8_t writeConfig(uint8_t config);
  uint8_t writeConfig(uint8_t scanMode, uint8_t channel, uint8_t adcMode);

  static const uint8_t REFERENCE_VDD            = 0x00; //Use VDD as reference voltage (5V/3V3)
  static const uint8_t REFERENCE_EXTERNAL       = 0x02; //Use external reference on pin (AINX/)REF
  static const uint8_t REFERENCE_INTERNAL       = 0x05; //Use internal reference (4.096V/2.048V) with external (AINX/)REF pin as ADC in
  static const uint8_t REFERENCE_INTERNAL_OUT   = 0x07; //use internal reference (4.096V/2.048V) and output reference on pin (AINX/)REF

  static const uint8_t CLOCK_INTERNAL           = 0x00; //Use the internal clock chip for conversion
  static const uint8_t CLOCK_EXTERNAL           = 0x01; //Use SCL as clock source

  static const uint8_t SETUP_KEEP_CONFIG        = 0x00; //Make no changes to the config byte during setup write
  static const uint8_t SETUP_RESET_CONFIG       = 0x01; //Rest config byte during setup-write

  static const uint8_t MODE_UNIPOLAR            = 0x00; //Run differential ADC in unipolar mode (only positive voltages from 0 to REF)
  static const uint8_t MODE_BIPOLAR             = 0x01; //Run differential ADC in bipolar mode (swing positive/negative around 1/2Vref), ignored in single-ended mode

  static const uint8_t SCANMODE_UPTO            = 0x00; //scans from AIN0 up to selected channel
  static const uint8_t SCANMODE_REPEATED        = 0x01; //converts selected channel 8 times
  static const uint8_t SCANMODE_UPTO_UPPERHALF  = 0x02; //scans up from AIN6(11608/09/10/11) or up from AIN2 (11606/07) to selected channel
  static const uint8_t SCANMODE_SINGLE          = 0x03; //converts selected channel once

  static const uint8_t ADCMODE_SINGLEENDED     = 0x01; //Configure ADC to run in single-ended mode
  static const uint8_t ADCMODE_DIFFERENTIAL    = 0x00; //Configure ADC to run in differential mode

private:
  int mAddress;
  uint8_t mAdcCount;
  uint8_t mRefVoltage;
  uint8_t readRegister(uint8_t reg, uint8_t *data);
  uint8_t readRegister(uint8_t reg, uint8_t *data, uint8_t len);
  uint8_t writeRegister(uint8_t reg, uint8_t value);
};

#endif //_MAX116xx_H