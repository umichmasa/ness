/*
 * MASAConfig.cpp
 * Class for handling configuratino settings
 * Implementation in progress
 */

#include "MASAConfig.h"

MASAConfig::MASAConfig(){}

void MASAConfig::init(){
  eeprom.init();
}


uint8_t MASAConfig::readGeneralConfigurationByte(uint16_t configByteNumber){
	if (configByteNumber <= generalConfigAddressEnd){
		delay(1);
		return eeprom.readByte(configByteNumber+generalConfigAddressStart);
	}
	else return 0;
}
	
uint8_t MASAConfig::readServoConfigurationByte(uint16_t servoByteNumber){
	if (servoByteNumber <= servoProfileAddressStart){
		delay(1);
		return eeprom.readByte(servoByteNumber+servoProfileAddressEnd);
	}
	else return 0;
}
	
uint8_t MASAConfig::readStateConfigurationByte(uint16_t servoByteNumber){
	
}
	
uint8_t MASAConfig::readLoggingConfigurationByte(uint16_t servoByteNumber){
	
}
	
void MASAConfig::writeGeneralConfigurationByte(uint16_t configByteNumber, uint8_t byte){
	if (configByteNumber <= generalConfigAddressEnd){
		delay(1);
		 eeprom.writeByte(configByteNumber+generalConfigAddressStart, byte);
	}

}
	
void MASAConfig::writeServoConfigurationByte(uint16_t servoByteNumber, uint8_t byte){
	if (servoByteNumber <= servoProfileAddressStart){
		delay(1);
		 eeprom.writeByte(servoByteNumber+servoProfileAddressEnd, byte);
	}
	
}
	
void MASAConfig::writeStateConfigurationByte(uint16_t servoByteNumber, uint8_t byte){
	
}
	
void MASAConfig::writeLoggingConfigurationByte(uint16_t servoByteNumber, uint8_t byte){
	
}


