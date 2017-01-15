#include "fram.h"
#include <Arduino.h>
#include <Wire.h>

FRAM::FRAM(uint8_t addr):
i2c_addr(addr)
{
}

void FRAM::begin() {
  Wire.begin();
}

void FRAM::read(uint16_t addr, void *pData, uint16_t len) {
  uint16_t i;
  uint8_t *next_data = pData;
  
  Wire.beginTransmission(i2c_addr);
  Wire.write(addr >> 8);
  Wire.write(addr & 0xFF);
  Wire.endTransmission();

  Wire.requestFrom(i2c_addr, len);

  for (i = 0; i < len; i++) {
    if (!Wire.available()) break;
    *next_data++ = Wire.read();
  }
}

void FRAM::write(uint16_t addr, const void *pData, uint16_t len) {
  uint16_t i;
  uint8_t *next_data = pData;

  Wire.beginTransmission(i2c_addr);
  Wire.write(addr >> 8);
  Wire.write(addr & 0xFF);

  for (i = 0; i < len; i++) {
    Wire.write(*next_data++);
  }

  Wire.endTransmission();  
}

