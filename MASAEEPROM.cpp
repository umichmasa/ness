/*
 * MASAEEPROM.cpp
 * Class for interfacing with I2C EEPROM chip
 */
 
#include "MASAEEPROM.h"

MASAEEPROM::MASAEEPROM(){}

void MASAEEPROM::init(){
  Wire.begin();
}

void MASAEEPROM::writeByte(uint16_t address, uint8_t b){
  uint8_t address_upper = address>>8;
  uint8_t address_lower = address;
  
  Wire.beginTransmission(WRITE_ADDRESS);
  Wire.write(address_upper);
  Wire.write(address_lower);
  Wire.write(b);
  Wire.endTransmission(true);
}

uint8_t MASAEEPROM::readByte(uint16_t address){
  uint8_t address_upper = address>>8;
  uint8_t address_lower = address;

  uint8_t read_in;
  
  Wire.beginTransmission(WRITE_ADDRESS);
  Wire.write(address_upper);
  Wire.write(address_lower);
  Wire.endTransmission(false);
  Wire.beginTransmission(READ_ADDRESS);
  Wire.requestFrom(READ_ADDRESS,1);
  read_in = Wire.read();
  Wire.endTransmission(true);

  return read_in;
  
}
