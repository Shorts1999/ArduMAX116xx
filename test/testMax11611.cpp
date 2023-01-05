#include <Arduino.h>

#include <MAX116xx.h>

MAX116xx maxAdc = MAX116xx(MAX11611_ADDRESS, &Wire);

void setup(){
    Serial.begin(115200);

    maxAdc.begin();
}

void loop(){
    // maxAdc.readAll();

    uint16_t adcValue = maxAdc.readADC(0);

    Serial.printf("Read value: %d; (%fV)", adcValue, (adcValue/1024.0f)*3.3f);
}