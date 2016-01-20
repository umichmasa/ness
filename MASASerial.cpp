/*
 * MASASerial.cpp
 * Packet wrapper for Serial class
 */
 


#include "MASASerial.h"
//#include <Arduino.h>

//FF AA LL BBBBBBBBBB CC DD
//FF SOP
//AA Header
//LL
//BB Data
//CC Checksum
//DD EOP

//SOP (one byte)
//FF

//Header (one byte)
//DD - Data request 
//ED - Update Notification 
//CC - State Change Notification
//DF - Shout (for connection)
//EB - Response (for connection)

//Length of data (one byte)
//LL

//Data (LL bytes)


//Checksum (one byte)
//FF default

//EOP
//EE



//Constructor
MASASerial::MASASerial(){
  
  SerialUSB.begin(BAUD,SERIAL_8O2);
  for (uint8_t i = 0; i < PACKET_SIZE; i++){
    bufferIn[i] = 0x00;
    bufferOut[i] = 0x00;
    
  }
}

bool MASASerial::sendPacket(uint8_t* packet, size_t pSize){
  bool sent = 0;
  for(uint8_t i = 0; i < pSize; i++){
    sent&=SerialUSB.write(*(packet+i));
  }
  
  return sent;
}

bool MASASerial::sendData(uint8_t header, uint8_t* data, size_t dSize){

  bufferOut[0] = SOP;
  bufferOut[1] = header;
  bufferOut[2] = dSize;
  for (uint8_t i = 0; i < dSize; i++)
    bufferOut[i+3] = *(data+i);
  bufferOut[dSize+3] = checksum(bufferOut, dSize);
  bufferOut[dSize+4] = EOP;
  sendPacket(bufferOut, OVERHEAD_SIZE+dSize);
}


uint8_t MASASerial::checksum(uint8_t* packet, size_t dSize){
  uint8_t sum = 0;
  
  for (uint8_t i = 0; i < 3 + dSize; i++){
    sum+=*(packet+i);
    
  }
  
  return sum;
    
}

void MASASerial::update(){

  if (!messageAvailable()){
    while(SerialUSB.available()){
      if (lastIndex<PACKET_SIZE) bufferIn[lastIndex++] = SerialUSB.read();
    }
    
    if (inputState(bufferIn, lastIndex)==RESET){
      resetBuffer();
    }else if (!inAvailable&&inputState(bufferIn, lastIndex)==COMPLETE){
      inAvailable = true;
      for (uint8_t i = 3; i < 3+bufferIn[2]; i++){
        dataIn.data[i-3] = bufferIn[i];
      }
        dataIn.header = bufferIn[1];
        dataIn.lengthData = bufferIn[2];
    }
    
  }else{
	if (SerialUSB.available() == 64) resetBuffer();
  
  }
  
  
}

uint8_t MASASerial::inputState(uint8_t* packet, size_t iSize){

  if (packet[0]==0x00) return INCOMPLETE; //First byte was reset
  
  if (packet[0]!=SOP) return RESET; //First byte is invalid
  
  if (packet[1] == 0x00) return INCOMPLETE; //Second byte was reset
  
  if (packet[2] == 0x00) return INCOMPLETE; //Third byte was reset
  
  if ((OVERHEAD_SIZE+packet[2]) > iSize)
	return INCOMPLETE; //Haven't found all data bytes

  if (((packet[2]+3)>PACKET_SIZE)||(packet[packet[2]+3]!=checksum(packet, packet[2])))
	return RESET; //Too large or breaks checksum
  
  if (packet[iSize-1]==EOP) return COMPLETE; //full packet
  
  return INCOMPLETE; //default
  
}




bool MASASerial::messageAvailable(){
  return inAvailable;
  
}


uint8_t* MASASerial::getMessage(){
  if (messageAvailable()){
    inAvailable = false;
    
    resetBuffer();
    
    return dataIn.data;
  
  } else 
    return NULL;
}

void MASASerial::resetBuffer(){
	uint8_t len = bufferIn[2];
	for(uint8_t i = 0; i < 3+len; i++){
		bufferIn[i] = 0x00;
	}
	lastIndex = 0;
  
}

size_t MASASerial::getMessageLength(){
  return dataIn.lengthData;
  
}

bool MASASerial::sendError(){
  SerialUSB.println("error");
  return false;
}

uint8_t MASASerial::getMessageHeader(){
	return dataIn.getHeader();
}

uint8_t MASASerialMessage::getHeader(){
  return header;
}

const uint8_t* MASASerialMessage::getData(){
  return data;
}

const uint8_t MASASerialMessage::getSize(){
  return lengthData;
  
}

