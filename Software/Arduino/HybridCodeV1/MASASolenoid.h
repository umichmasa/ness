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
  //Output pin
	uint8_t controlPin;

  //n/a
	uint8_t currentPin;

  //n/a
	uint8_t currentAvailable;

  //Has control started
	uint8_t pinAttached;

  //State of solenoid
  uint8_t state;

  //Time solenoid has been on or off
	uint32_t timeOnMillis;
	uint32_t timeCoolingMillis;

  //Time solenoid turned on or off
  uint32_t timeOnStartMillis;
  uint32_t timeCoolingStartMillis;

  //n/a
	uint32_t currentDraw;

  //Has solenoid finished cooling
	uint8_t cooledOff;

  //n/a
	uint8_t calculateCurrent();
	
  public:

    //Constructor
    MASASolenoid();

       //State constants
	static const uint8_t OPEN = 1;
	static const uint8_t CLOSED = 0;
	static const uint8_t COOLING = 2;
	static const uint32_t MAX_TIME_ON = 15000;
	static const uint32_t COOL_OFF_TIME = 15000;
	
	//Set pins
	void setControlPin(uint8_t c);
	void setCurrentPin(uint8_t c);

  //Called periodically to check state
	void update(uint32_t time);

  //Get state
	uint8_t isOpen();
	uint8_t isClosed();
	uint8_t isCooling();
  uint8_t getState();

  //Control solenoid
	uint8_t open();
	uint8_t close();

  //Get timing
  uint32_t timeOn();
	uint32_t timeCooling();

  //Get pins
	uint8_t getControlPin();
	uint8_t getCurrentPin();

  //n/a
	uint16_t getCurrent();
	
};


#endif
