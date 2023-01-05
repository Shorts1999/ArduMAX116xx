#include <Arduino.h>

#include <MAX116xx.h>

MAX116xx maxAdc = MAX116xx(MAX11611_ADDRESS);

void i2cScan(){
    Wire.begin();
    for(uint8_t address=0; address<127; address++){
        Wire.beginTransmission(address);
        if(!Wire.endTransmission()){
            Serial.printf("Found device at address 0x%02X\n", address);
            return;
        }
    }
    Serial.println("No i2c device found!");
}

void setup(){
    Serial.begin(115200);



    maxAdc.begin();
    i2cScan();
    //Write the setup byte:
    uint8_t setupByte = 0b10000000; //Write all setup bytes to 0
    maxAdc.writeConfig(setupByte);


    //write configuration byte:
    uint8_t configByte = 0b01100111;    //Read only ADC3
    maxAdc.writeConfig(configByte);
}

void loop(){
    // maxAdc.readAll();

    uint16_t adcValue = maxAdc.readADC(0);

    Serial.printf("Read value: \t %d \t\t(0x%04X); \t\t(%fV)\n", adcValue, adcValue, (adcValue/1024.0f)*3.3f);
    delay(500);
}