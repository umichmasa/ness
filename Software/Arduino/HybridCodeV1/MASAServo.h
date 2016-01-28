/*
 * MASAServo.h
 * Functional wrapper for Servo class
 * Allows for extra functionality to servo, including pattern operation
 */

/*Wrapper for hobby servo control
* VarSpeedServo isn't written for ARM, so
* it's just easier to just wrap Servo (which thankfully has a SAMD implementation)
*
* This will make throttling easier, trust me
*
* If someone is so inclined, they can modify the SAMD Servo class themselves
* Once I have some experience with the architecture I'll give it a shot too
*/

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

//Struct to hold a single command to a servo
struct MASAServoCommand{
	uint8_t angle;
	uint16_t time;
	double velocity;
};

//Main Class
class MASAServo{

  static const uint8_t MAX_COMMAND_LENGTH = 64;
  
  private:
  //Control pin, and whether it has been set
	uint8_t controlPin;
	uint8_t controlAvailable;

  //Current read pin (not used
	uint8_t currentPin;
	uint8_t currentAvailable;

  //Servo object
	Servo servo;

  //Last time servo has been updated
	uint32_t lastTime;

  //Array of commands
	MASAServoCommand commands[MAX_COMMAND_LENGTH];
	uint8_t commandLength;
	uint8_t commandIndex;

  //Command sequence complete
	uint8_t commandComplete;

  //Mode running (sequence, normal, etc)
	uint8_t mode;

  //Output value
	uint8_t output;
	
	uint16_t currentDraw;

  //Time sequence started
  uint32_t timeStart;

  //Command to start sequence
  uint8_t goCommand;

  
  public:

  //Constructor
  MASAServo();

  //Attach control pin
	void attach(uint8_t c);

  //Write an angle in normal operation
  void writeAngle(uint8_t angle);
	
	//Call periodically to update commands
	void update(uint32_t time);


  //Add a new command
	uint8_t addCommand(uint8_t angle, uint32_t time);

	void resetCommands();
	void clearCommands();

  //Set mode
	void modeNormal();
	void modeCommand();
  uint8_t getMode();

  //Get current output to servo
	uint8_t getOutput();
	
	uint16_t getCurrent();
	
	uint8_t calculateCurrent();

  //Start command procedure
  void startCommand();

  //Has commands been started
  uint8_t commandStarted();

  //Has command finished
  uint8_t commandDone();

  //Defined by servo
	static const uint8_t OPEN = 180;
	static const uint8_t CLOSED = 0;
	static const uint8_t ESTIMATED_TURN_RATE = 70; //deg/s (test manually, at 7.4V)
	
	
	//Modes
	static const uint8_t NORMAL = 0;
	static const uint8_t COMMAND = 1;
	static const uint8_t FUNCTION = 3;
	
	
};


#endif
