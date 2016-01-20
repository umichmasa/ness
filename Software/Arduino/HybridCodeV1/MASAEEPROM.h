/*
 * MASAEEPROM.h
 * Class for interfacing with I2C EEPROM chip
 */
 
#ifndef MASAEEPROM_h
#define MASAEEPROM_h

#include <Arduino.h>
#include <Wire.h>

// Flash module commands:
const uint8_t READ_ADDRESS = 0b1010000;
const uint8_t WRITE_ADDRESS = 0b1010000;

//const uint8_t ADDRESS_BITS = 13;

const uint16_t NUMBER_BYTES = 8000;

class MASAEEPROM
{
  public:
    MASAEEPROM();
    void init();
    void eraseChip();
    
    uint8_t readByte(uint16_t address);
    
    void writeByte(uint16_t address, uint8_t b);
    
  private:
    

};

//extern EEPROMChip EEPROM;

#endif

