/*
 * MASASolenoid.h
 * Definition for Solenoid Control + Current Sense
 * Allows for software protection of solenoid
 */



/*
TODO
-Calculation for solenoid thermal model
	-Use instead of time based cooling
	-Use model for servo as well
-Switch pins to PWM after confirming it doesn't kill board
	-Add current control function

*/




#ifndef MASASolenoid_h
#define MASASolenoid_h


#include <inttypes.h>
#include <stdlib.h>
#include <Arduino.h>

#define VERSION_NUMBER 1 //Original Release



//Main Class
class MASASolenoid{
  
  private:
    
	uint8_t controlPin;
	
	uint8_t currentPin;
	
	uint8_t currentAvailable;
	
	uint8_t pinAttached;
	
  uint8_t state;
	
	uint32_t timeOnMillis;
	uint32_t timeCoolingMillis;

  uint32_t timeOnStartMillis;
  uint32_t timeCoolingStartMillis;
  
	uint32_t currentDraw;
	
	uint8_t cooledOff;
	
	uint8_t calculateCurrent();
	
  public:
    MASASolenoid();
       
	static const uint8_t OPEN = 1;
	static const uint8_t CLOSED = 0;
	static const uint8_t COOLING = 2;
	static const uint32_t MAX_TIME_ON = 15000;
	static const uint32_t COOL_OFF_TIME = 15000;
	
	
	void setControlPin(uint8_t c);
	void setCurrentPin(uint8_t c);
	
	void update(uint32_t time);
	
	uint8_t isOpen();
	uint8_t isClosed();
	uint8_t isCooling();

  uint8_t getState();
  
	uint8_t open();
	uint8_t close();
  uint32_t timeOn();
	uint32_t timeCooling();
	uint8_t getControlPin();
	uint8_t getCurrentPin();
	
	uint16_t getCurrent();
	
};


#endif
