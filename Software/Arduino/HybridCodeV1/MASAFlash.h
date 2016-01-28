/*
 * MASAFlash.h
 * Class for interfacing with SPI Flash chip
 * To be used with MASALogging
 * Hardware Interfacing
 * Look up datasheet for SST25VF016B-75-4I-S2AF
 */
 


#ifndef MASAFlash_h
#define MASAFlash_h

#include <Arduino.h>
#include <SPI.h>

// Chip register constants:
const byte BYTE_WRITE =       0x02;  // Command to program a byte
const byte READ_SLOW =        0x03;  // Read command
const byte READ_FAST =        0x0B;  // Read command  
const byte WRITE_ENABLE =     0x06;  // Write enable command
const byte WRITE_DISABLE =    0x04;  // Write disable command
const byte CHIP_ERASE =       0xC7;  // Command to erase entire chip
const byte SECTOR_ERASE_4K =  0x20;  // Command to erase 4 KByte of memory array
const byte SECTOR_ERASE_32K = 0x52;  // Command to erase 4 KByte of memory array
const byte SECTOR_ERASE_64K = 0xD8;  // Command to erase 4 KByte of memory array
const byte STATREG_READ =     0x05;  // Command to read status registry
const byte READ_ID =          0x9F;  // Command to read ID (JEDEC)
const byte STATREG_WR_EN =    0x50;  // Command to set write enable on status registry
const byte STATREG_WRITE =    0x01;  // Command to write status registry



// Clock divider
const byte CLOCK_DIVIDER =  2;     // Clock divider (84MHz/CLOCK_DIVIDER=21Mhz operations clock)

//Memory area constants
#define  SECTOR_4KB    0
#define  SECTOR_32KB   1
#define  SECTOR_64KB   2
#define  READ_BEGIN    3
#define  READ_CONTINUE 4
#define  READ_END      5



class MASAFlash
{
  public:
    //Constructor
    MASAFlash();

    //Initialization
    void init(byte slaveSelectPin);
    
    //Wipe entire chip
    void eraseChip();

    //Wipe specified sector of chip
    void eraseSector(long sectorAddress, byte sectorSize = SECTOR_4KB);
    
    //Ask chip if it is done with read/write operations
    boolean isReady();

    //Read byte from address
    byte readByte(long address);

    //Read multiple bytes from address (in chain)
    byte readByteMulti(long address, byte action = READ_BEGIN);

    //Write byte to address
    void writeByte(byte b, long address);

    //Read chip ID (not terribly useful)
    void readID(byte *id);

    //Read from the configuration registers
    byte readStatusRegistry();

    //Write to the configuration registers (see datasheet)
    void writeStatusRegistry(byte registry);

    //Enable writing to chip
    void write_enable();

    //Disable writing to chip
    void write_disable();
  private:

      //Enable writing to status registers
    byte statreg_write_enable();    
    
  private:
    byte CE;  // Chip enable (SPI Chip Select) pin 
};
#endif
