#include <Arduino.h>

#include <MAX116xx.h>

MAX116xx maxAdc = MAX116xx(MAX11611_ADDRESS);

void i2cScan() {
    Wire.begin();
    for (uint8_t address = 0; address < 127; address++) {
        Wire.beginTransmission(address);
        if (!Wire.endTransmission()) {
            Serial.printf("Found device at address 0x%02X\n", address);
            return;
        }
    }
    Serial.println("No i2c device found!");
}

void testReadSingle(uint8_t channelNum) {
    uint16_t result = maxAdc.readADC(channelNum);

    Serial.printf("Received [%i]: %2x (%1.2fV)\n", channelNum, result, (result / 1024.0f) * 3.3f);
}

void testReadAll() {
    const uint8_t adcCount = maxAdc.getAdcCount();
    Serial.printf("Reading %i ADCs\n", adcCount);
    uint8_t adcData[2*adcCount];

    maxAdc.readAll(adcData);

    for(uint8_t i =0; i<2*maxAdc.getAdcCount(); i+=2){
        uint16_t result = (adcData[i]<<8 | adcData[i+1]) &0x03ff;
            Serial.printf("Received [%i]: %2x (%1.2fV)\n", i, result, (result / 1024.0f) * 3.3f);
    }
}

void setup() {
    Serial.begin(115200);
    maxAdc.begin();

    maxAdc.writeSetup(MAX116xx::REFERENCE_VDD, MAX116xx::CLOCK_INTERNAL, MAX116xx::MODE_UNIPOLAR, MAX116xx::SETUP_KEEP_CONFIG);

    Serial.println("-----STARTING ADC TEST--------\n\n");

}

void loop() {
    Serial.println("\n-----------------SINGLE ADCs---------\n");
    for(uint8_t i=0; i<12; i++){
        testReadSingle(i);
    }
    Serial.println("\n-------------ADC READALL-------------\n");
    testReadAll();
    delay(10000);
}