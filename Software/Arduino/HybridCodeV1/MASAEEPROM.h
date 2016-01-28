/*
 * MASAEEPROM.h
 * Class for interfacing with I2C EEPROM chip
 * To be used with MASAConfig to write files
 * Hardware interfacing
 * Look up datasheet for 24AA65/SM
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

    //Constructor
    MASAEEPROM();

    //Chip communication initialization
    void init();

    //Wipe EEPROM chip
    void eraseChip();

    //Bytewise read from memory address
    uint8_t readByte(uint16_t address);

    //Bytewise write from memorya ddress
    void writeByte(uint16_t address, uint8_t b);
    
  private:
    

};

#endif

