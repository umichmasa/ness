/*
 * MASAServo.cpp
 * Functional wrapper for Servo class
 */

#include "MASAServo.h"
#include <Servo.h>


MASAServo::MASAServo(){
	
	controlAvailable = 0;
	
	
	currentAvailable = 0;
	
	lastTime = 0;

  timeStart = 0;
	
	commandLength = 0;
	commandIndex = 0;
	commandComplete = 0;
	
	output = CLOSED;

  goCommand = 0;
	
	mode = NORMAL;
}


void MASAServo::attach(uint8_t c){
	servo.attach(c);
	controlAvailable = 1;
}


void MASAServo::writeAngle(uint8_t angle){
	if (mode==NORMAL) output = angle;
	
	if (angle > OPEN) servo.write(OPEN);
	else if (angle < CLOSED) servo.write(CLOSED);
	else servo.write(angle);
}

void MASAServo::update(uint32_t time){
	if (mode == COMMAND && !commandComplete && goCommand){
    //SerialUSB.println("Called");
 		//SerialUSB.println((int)((time-timeStart)*commands[commandIndex].velocity));
    //SerialUSB.println(((time-timeStart)*commands[commandIndex].velocity));
    uint32_t velocityComponent = (int)((time - timeStart) * commands[commandIndex].velocity);
    uint32_t previousComponent = (commandIndex!=0)?commands[commandIndex-1].angle:0;
    //SerialUSB.print(" + ");
    //SerialUSB.println((commandIndex!=0)?commands[commandIndex-1].angle:0);
 		output = velocityComponent + previousComponent;
		//SerialUSB.println(output);
    //SerialUSB.println((float)((time - timeStart) * commands[commandIndex].velocity) + (float)(commandIndex!=0)?commands[commandIndex-1].angle:0);
		//SerialUSB.println(commands[commandIndex].velocity);
    //SerialUSB.println(commandIndex);

    //SerialUSB.println(time-timeStart);
    //SerialUSB.println(output);
    //SerialUSB.println();
		if ((time-timeStart)>=(commands[commandIndex].time)){
		  //SerialUSB.println("Command++");
		  timeStart+=commands[commandIndex].time;
		  commandIndex++;
		}
		if (commandIndex>commandLength) commandComplete = 1;
		//timeStart = time;
		writeAngle(output);
	}
	
	
	
}

void MASAServo::resetCommands(){
	commandIndex = 0;
}

void MASAServo::clearCommands(){
	for (int i = 0; i < commandLength; i++){
		commands[i].velocity = 0;
    commands[i].angle = 0;
    commands[i].time = 0;
	}
}

void MASAServo::modeNormal(){
	mode = NORMAL;
}
void MASAServo::modeCommand(){
	mode = COMMAND;
}

uint8_t MASAServo::getMode(){
  return mode;
}

uint8_t MASAServo::getOutput(){
	return output;
}

uint8_t MASAServo::calculateCurrent(){
	if (!currentAvailable) return 0;
	
	currentDraw = analogRead(currentPin)/4096.0 * 10.0; //Current in amps
	
	return 1;
}

uint16_t MASAServo::getCurrent(){
	return currentDraw;
}

void MASAServo::startCommand(){
  goCommand = 1;
  mode = COMMAND;
  if (timeStart==0) timeStart=millis();

  //for (int i = 0; i < commandLength; i++){
  //  SerialUSB.print(commands[i].angle); SerialUSB.print(" "); SerialUSB.print(commands[i].time); SerialUSB.print(" "); SerialUSB.print(commands[i].velocity); SerialUSB.println(" ");
  //}

  
}

uint8_t MASAServo::commandStarted(){
  return goCommand;
}

uint8_t MASAServo::commandDone(){
  if (commandIndex>(commandLength))
  return 1;
}

uint8_t MASAServo::addCommand(uint8_t angle, uint32_t time){
	if (commandLength >= MAX_COMMAND_LENGTH-1){
		return -1;
	}
	
	
	MASAServoCommand m;
	m.angle = angle + ((commandLength!=0)?commands[commandLength-1].angle:0);
  //SerialUSB.println(m.angle);
  //SerialUSB.println((commandLength!=0)?commands[commandLength-2].angle:0);
	m.time = time;
	//m.velocity = (double)(m.angle-((commandLength!=0)?commands[commandLength-2].angle:0))/(double)time;

  //if (commandLength > 0)
  //commands[commandLength-1].velocity = ((float)(m.angle-commands[commandLength-1].angle))/((float)(commands[commandLength-1].time));
  
  m.velocity = (float)angle/(float)time;
  
  //SerialUSB.print(angle); SerialUSB.print(" "); SerialUSB.print(m.angle); SerialUSB.print(" "); SerialUSB.print(m.velocity); SerialUSB.println(" ");
	commands[commandLength] = m;
	
	commandLength++;
	
	
	return 1;
	
}

