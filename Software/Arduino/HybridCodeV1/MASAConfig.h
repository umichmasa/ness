/*
 * MASAConfig.h
 * Class for handling configuratino settings
 * Implementation in progress
 */


//Configuration definitions for 8kBytes of EEPROM memory
//Memory written/read in bytes


//Have laptop initiate write command; send entire configuration bytewise



#ifndef MASAConfig_h
#define MASAConfig_h

#include <Arduino.h>
#include "MASAEEPROM.h"


class MASAConfig
{
  public:
    MASAConfig();
    MASAEEPROM eeprom;

	
	uint8_t readGeneralConfigurationByte(uint16_t configByteNumber);
	
	uint8_t readServoConfigurationByte(uint16_t servoByteNumber);
	
	uint8_t readStateConfigurationByte(uint16_t servoByteNumber);
	
	uint8_t readLoggingConfigurationByte(uint16_t servoByteNumber);
	
	void writeGeneralConfigurationByte(uint16_t configByteNumber, uint8_t byte);
	
	void writeServoConfigurationByte(uint16_t servoByteNumber, uint8_t byte);
	
	void writeStateConfigurationByte(uint16_t servoByteNumber, uint8_t byte);
	
	void writeLoggingConfigurationByte(uint16_t servoByteNumber, uint8_t byte);
	
	void init();

  static const uint8_t GENERAL_CONFIG = 0x00;
  static const uint8_t SERVO_CONFIG = 0x01;
  
  private:
  
  const static uint16_t generalConfigAddressStart = 0;
  const static uint16_t generalConfigAddressEnd = 999;
  const static uint16_t generalConfigBytesUsed = 10;
  
  const static uint16_t servoProfileAddressStart = 1000;
  const static uint16_t servoProfileAddressEnd = 1999;
  const static uint16_t servoProfileBytesUsed = 100;
  
  const static uint16_t stateConfigAddressStart = 2000;
  const static uint16_t stateConfigAddressEnd = 2999;
  const static uint16_t stateConfigBytesUsed = 500;
  
  const static uint16_t dataLoggingConfigAddressStart = 3000;
  const static uint16_t dataLoggingConfigAddressEnd = 3999;
  const static uint16_t dataLoggingBytesUsed = 50;
  

  
  

  
};

/*


Purpose						Byte

General Configuration       Load from laptop
Closed Servo				0
Open Servo					1
Over Pressure Upper 		2
Over Pressure Lower 		3
Send Frequency Lower 		4
Send Frequency Upper		5
Receive Frequency Lower 	6
Receive Frequency Upper		7
Cooling Enabled 			8
Wireless Baud Lower			9
Wireless Baud Upper     	10



Servo Profile				Load from laptop
Number of commands			0 (n is command (0 indexed))
Command Angle			    	3*n+1
Command Time Lower			3*n+2
Command Time Upper			3*n+3



State Config				Reconfigure during operation (infrequently). Things that will be lost if reset.
Connected State				0
Unlocked State				1
Servo Command				2



*/





#endif

