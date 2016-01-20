/*
 * MASASerial.h
 * Packet wrapper for Serial class
 */


/*
TODO
	1. Add error communication option
	2. Define standard messages
	3. All the documentation + comments
		4.1. Define MASA documentation standard
*/




#ifndef MASASerial_h
#define MASASerial_h


#include <inttypes.h>
#include <stdlib.h>
#include <Arduino.h>

#define VERSION_NUMBER 1 //Original Release


#define PACKET_SIZE  64
#define OVERHEAD_SIZE 5
#define DATA_SIZE  PACKET_SIZE - OVERHEAD_SIZE
#define BAUD 9600


class MASASerialMessage{
  private:
    friend class MASASerial;
    uint8_t header;
    uint8_t data[DATA_SIZE];
    size_t lengthData;
  public:
    uint8_t getHeader();
    const uint8_t* getData();
    const uint8_t getSize();
  
};

//Main Class
class MASASerial{
  
  private:
    
    uint8_t sizeBufferIn;
    uint8_t sizeBufferOut;
    
    uint8_t bufferIn[PACKET_SIZE];
    uint8_t bufferOut[PACKET_SIZE];
	
    uint8_t checksum(uint8_t* packet, size_t dSize);
    
    uint8_t inputState(uint8_t* packet, size_t iSize);
    
    uint8_t lastIndex = 0;
    
    MASASerialMessage dataIn;
    
    bool ack = false;
    
    bool sendPacket(uint8_t* packet, size_t pSize);
    
    bool inAvailable = false;
    
	void resetBuffer();

    uint8_t type = 0;
  
  public:
    MASASerial();
        
    bool sendData(uint8_t header, uint8_t* data, size_t dSize);
    
    uint8_t* getMessage();
    size_t getMessageLength();
    uint8_t getMessageHeader();
	
	  void setType(uint8_t type);
	
    bool sendError();
    bool messageAvailable();
    
    void update();
    
    static const uint8_t SOP = 0xFF;
    static const uint8_t DATA_REQUEST = 0xDD;
    static const uint8_t DATA_RESP = 0xDE;
    static const uint8_t ACK_RESP = 0xAC;
    static const uint8_t UPDATE_NOTIF = 0xED;
    static const uint8_t STATE_NOTIF = 0xCC;
    static const uint8_t SHOUT = 0xDF;
    static const uint8_t SHOUT_RESP = 0xEB;
    static const uint8_t ERROR_RESP = 0xEE;
    static const uint8_t EOP = 126;

    static const uint8_t CONFIG = 0xCF;
    static const uint8_t CONFIG_RESPONSE = 0xD0;
    
    static const uint8_t RESET = 0x00;
    static const uint8_t INCOMPLETE = 0x01;
    static const uint8_t COMPLETE = 0x02;
    
	
	  static const uint8_t MAX_PACKET_SIZE = PACKET_SIZE;
	  static const uint8_t MAX_DATA_SIZE = DATA_SIZE;
	
    static const uint8_t USB_COMM = 0;
    static const uint8_t SERIAL_COMM = 1;
    static const uint8_t SERIAL1_COMM = 2;
    

    
};





#endif
