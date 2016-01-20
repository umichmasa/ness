/*
 * MASAServo.cpp
 * Functional wrapper for Servo class
 */

//Wrapper for hobby servo control
//VarSpeedServo isn't written for ARM, so
//it's just easier to just wrap Servo (which thankfully has a SAMD implementation)

//This will make throttling easier, trust me

//If someone is so inclined, they can modify the SAMD Servo class themselves
//Once I have some experience with the architecture I'll give it a shot too


/*
TODO
	Figure out why timing is off?
*/




#ifndef MASAServo_h
#define MASAServo_h


#include <inttypes.h>
#include <stdlib.h>
#include <Arduino.h>
#include <Servo.h>
#define VERSION_NUMBER 1 //Original Release


struct MASAServoCommand{
	uint8_t angle;
	uint16_t time;
	double velocity;
};

//Main Class
class MASAServo{

  static const uint8_t MAX_COMMAND_LENGTH = 64;
  
  private:
	uint8_t controlPin;
	uint8_t controlAvailable;
	
	uint8_t currentPin;
	uint8_t currentAvailable;
	
	Servo servo;
    
	uint32_t lastTime;
	
	MASAServoCommand commands[MAX_COMMAND_LENGTH];
	uint8_t commandLength;
	uint8_t commandIndex;
	
	uint8_t commandComplete;
	
	uint8_t mode;
	
	uint8_t output;
	
	uint16_t currentDraw;


  uint32_t timeStart;


  uint8_t goCommand;

  
  public:
  MASAServo();
	
	void attach(uint8_t c);
	
  void writeAngle(uint8_t angle);
	
	
	void update(uint32_t time);
	
	uint8_t addCommand(uint8_t angle, uint32_t time);
	
	void resetCommands();
	void clearCommands();
	
	void modeNormal();
	void modeCommand();
  uint8_t getMode();
  
	uint8_t getOutput();
	
	uint16_t getCurrent();
	
	uint8_t calculateCurrent();
	
  void startCommand();

  uint8_t commandStarted();
  
  uint8_t commandDone();
  
	static const uint8_t OPEN = 180;
	static const uint8_t CLOSED = 0;
	static const uint8_t ESTIMATED_TURN_RATE = 70; //deg/s (test manually, at 7.4V)
	
	
	
	static const uint8_t NORMAL = 0;
	static const uint8_t COMMAND = 1;
	static const uint8_t FUNCTION = 3;
	
	
};


#endif
