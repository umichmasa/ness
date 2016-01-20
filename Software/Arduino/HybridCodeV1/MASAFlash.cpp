/*
 * MASAFlash.cpp
 * Class for interfacing with SPI Flash chip
 */
 

#include "MASAFlash.h"

MASAFlash::MASAFlash(void)
{
  
}

void MASAFlash::init(byte slaveSelectPin)
{
  CE = slaveSelectPin;
  pinMode(CE,OUTPUT);
  // start the SPI library:
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  //SPI.setClockDivider(CLOCK_DIVIDER);  
}
    
byte MASAFlash::readByte(long address)
{
  byte read_byte = 0x00;
  byte address_b[4];
  address_b[0] = address;
  address_b[1] = address >> 8;
  address_b[2] = address >> 16;
  address_b[3] = address >> 24;
  
  digitalWrite(CE,LOW);
  SPI.transfer(READ_SLOW);  // Read command
  SPI.transfer(address_b[2]);  // Address MSB [A23-A16]
  SPI.transfer(address_b[1]);  // Address MSB [A15-A8]
  SPI.transfer(address_b[0]);  // Address MSB [A7-A0]  
  //SPI.transfer(0x00);  // Dummy byte
  read_byte =  SPI.transfer(0x00); // Read byte from SPI bus

  digitalWrite(CE,HIGH);
  return read_byte;
}

byte MASAFlash::readByteMulti(long address, byte action)
{
  byte read_byte = 0x00;
  byte address_b[4];
  
  switch(action)
  {
    case READ_BEGIN:
      address_b[0] = address;
      address_b[1] = address >> 8;
      address_b[2] = address >> 16;
      address_b[3] = address >> 24;
      digitalWrite(CE,LOW);
      SPI.transfer(READ_SLOW);  // Read command
      SPI.transfer(address_b[2]);  // Address MSB [A23-A16]
      SPI.transfer(address_b[1]);  // Address MSB [A15-A8]
      SPI.transfer(address_b[0]);  // Address MSB [A7-A0]  
      SPI.transfer(0x00);  // Dummy byte
      read_byte =   SPI.transfer(0x00);  
      break;
      
    case READ_CONTINUE:
      read_byte =   SPI.transfer(0x00);  
      break;

    case READ_END:
      read_byte =   SPI.transfer(0x00); 
      break;
  }
  digitalWrite(CE,HIGH);
  return read_byte;  
}

void MASAFlash::writeByte(byte b, long address)
{
  byte address_b[4];
  address_b[0] = address;
  address_b[1] = address >> 8;
  address_b[2] = address >> 16;
  address_b[3] = address >> 24;
  //digitalWrite(CE,LOW);
  //write_enable();
  //digitalWrite(CE,HIGH);
  
  digitalWrite(CE,LOW);
  SPI.transfer(BYTE_WRITE);  // Read command
  SPI.transfer(address_b[2]);  // Address MSB [A23-A16]
  SPI.transfer(address_b[1]);  // Address MSB [A15-A8]
  SPI.transfer(address_b[0]);  // Address MSB [A7-A0]  
  SPI.transfer(b);  // Data byte
  digitalWrite(CE,HIGH);
}


byte MASAFlash::readStatusRegistry()
{
  byte statreg;
  digitalWrite(CE,LOW);
  SPI.transfer(STATREG_READ);
  statreg = SPI.transfer(0x00); 
  digitalWrite(CE,HIGH); 
  return statreg;
}

void MASAFlash::writeStatusRegistry(byte registry)
{
  digitalWrite(CE,LOW);
  statreg_write_enable();
  digitalWrite(CE,HIGH);
  delay(10);
  digitalWrite(CE,LOW);
  SPI.transfer(STATREG_WRITE);
  SPI.transfer(registry);
  digitalWrite(CE,HIGH);
}

byte MASAFlash::statreg_write_enable()
{
  SPI.transfer(STATREG_WR_EN);
}    

    
void MASAFlash::eraseChip(void)
{
  digitalWrite(CE,LOW);
  write_enable();
  digitalWrite(CE,HIGH);
  delay(10);
  digitalWrite(CE,LOW);
  SPI.transfer(CHIP_ERASE);  
  digitalWrite(CE,HIGH);
}

boolean MASAFlash::isReady(void)
{
  byte statreg = readStatusRegistry();
  return bitRead(statreg,0);
}

void MASAFlash::eraseSector(long sectorAddress, byte sectorSize)
{
  byte address_b[4];
  address_b[0] = sectorAddress;
  address_b[1] = sectorAddress >> 8;
  address_b[2] = sectorAddress >> 16;
  address_b[3] = sectorAddress >> 24;
  digitalWrite(CE,LOW);
  write_enable();
  switch(sectorSize)
  {
    case SECTOR_4KB:
      SPI.transfer(SECTOR_ERASE_4K);  // 4KB sector erase
      break;  

    case SECTOR_32KB:
      SPI.transfer(SECTOR_ERASE_32K);  // 32KB sector erase
      break;  

    case SECTOR_64KB:
      SPI.transfer(SECTOR_ERASE_64K);  // 64KB sector erase
      break;        
  } 
  SPI.transfer(address_b[2]);  // Address MSB [A23-A16]
  SPI.transfer(address_b[1]);  // Address MSB [A15-A8]
  SPI.transfer(address_b[0]);  // Address MSB [A7-A0]  
  digitalWrite(CE,HIGH);
}


void MASAFlash::readID(byte *id)
{
  digitalWrite(CE,LOW);
  SPI.transfer(READ_ID);  
  id[0] = SPI.transfer(0x00);
  id[1] = SPI.transfer(0x00);
  id[2] = SPI.transfer(0x00);
  digitalWrite(CE,HIGH);
}

void MASAFlash::write_enable()
{
  
  SPI.transfer(WRITE_ENABLE);  
}

void MASAFlash::write_disable()
{
  SPI.transfer(WRITE_DISABLE); 
}

//Flash flash;

