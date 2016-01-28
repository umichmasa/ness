/*
 * MASASerial.h
 * Packet wrapper for Serial class
 * Groups messages together in standard format
 * Allows for simplified communication with MATLAB with
 * a non-ASCII interface
 * Communicates with MATLAB version of this program
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


#define PACKET_SIZE  64 //Size of full packet (bytes)
#define OVERHEAD_SIZE 5 //Size of overhead in a message
#define DATA_SIZE  PACKET_SIZE - OVERHEAD_SIZE //Number of data bytes
#define BAUD 9600 //

//Container for message data
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

    //Size of buffers (did I stop using these?)
    uint8_t sizeBufferIn;
    uint8_t sizeBufferOut;

    //Buffer declaration
    uint8_t bufferIn[PACKET_SIZE];
    uint8_t bufferOut[PACKET_SIZE];

    //Calculation based on data in packet; form of error checking
    uint8_t checksum(uint8_t* packet, size_t dSize);

    //Get current state of buffer in (check for valid packet)
    uint8_t inputState(uint8_t* packet, size_t iSize);

    //Last packet read index (in buffer)
    uint8_t lastIndex = 0;

    //Buffer for holding data input
    MASASerialMessage dataIn;

    //Not used
    bool ack = false;

    //Transmit a packet
    bool sendPacket(uint8_t* packet, size_t pSize);

    //Is there an available message to read?
    bool inAvailable = false;

    //Clean out the buffer
	void resetBuffer();

    //type of transmitter
    //Will be implemented to switch between available serial protocols
    //IE, SAMD21 has SerialUSB, Serial, and Serial1
    uint8_t type = 0;
  
  public:

    //Constructor
    MASASerial();

    //Transmit a message (creates a packet)
    bool sendData(uint8_t header, uint8_t* data, size_t dSize);

    //Get pointer to data in buffer
    uint8_t* getMessage();

    //Get length of message
    size_t getMessageLength();

    //Get header from message
    uint8_t getMessageHeader();

    //Set type of communication (not used)
	  void setType(uint8_t type);

    //Send error message (not used)
    bool sendError();

    //Is a message available?
    bool messageAvailable();

    //Must be called periodically in loop to find in messages
    void update();

    //Header constants
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

    //State constants
    static const uint8_t RESET = 0x00;
    static const uint8_t INCOMPLETE = 0x01;
    static const uint8_t COMPLETE = 0x02;
    

    //Size constants
	  static const uint8_t MAX_PACKET_SIZE = PACKET_SIZE;
	  static const uint8_t MAX_DATA_SIZE = DATA_SIZE;

    //Protocol constants
    static const uint8_t USB_COMM = 0;
    static const uint8_t SERIAL_COMM = 1;
    static const uint8_t SERIAL1_COMM = 2;
    

    
};





#endif
