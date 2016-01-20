/*
 * MASASolenoid.cpp
 * Definition for Solenoid Control + Current Sense
 * Allows for software protection of solenoid
 */


#include "MASASolenoid.h"

//Constructor

MASASolenoid::MASASolenoid(){
    
	currentAvailable = 0;
	
	pinAttached = 0;
	
  state = 0;
	
	timeOnMillis = 0;
  timeOnStartMillis = 0;
  
	timeCoolingMillis = 0;
	timeCoolingStartMillis = 0;
 
	currentDraw = 0;

}


void MASASolenoid::update(uint32_t time){
	if ((state == OPEN) && ((timeOnMillis) > MAX_TIME_ON)){
		close();
    if (timeCoolingStartMillis==0)
      timeCoolingStartMillis = time;
		state = COOLING;
		//timeCoolingMillis = time-timeCoolingStartMillis;
		timeOnMillis = 0;
    timeOnStartMillis = 0;
	}
	
	if ((state == COOLING) && ((timeCoolingMillis) > COOL_OFF_TIME)) {
		state = CLOSED;
		timeCoolingMillis = 0;
    timeCoolingStartMillis = 0;
	}
	
	calculateCurrent();

 if (state==OPEN) timeOnMillis = time - timeOnStartMillis;
 if (state==COOLING)timeCoolingMillis = time - timeCoolingStartMillis;
	
}

void MASASolenoid::setControlPin(uint8_t c){
	controlPin = c;
  pinMode(controlPin,OUTPUT);
  digitalWrite(controlPin,LOW);
	pinAttached = 1;
}

void MASASolenoid::setCurrentPin(uint8_t c){
	currentPin = c;
	currentAvailable = 1;
}

uint8_t MASASolenoid::isOpen(){
	return state==OPEN;
}

uint8_t MASASolenoid::isClosed(){
	return state==CLOSED;
}

uint8_t MASASolenoid::isCooling(){
	return state==COOLING;
}

uint8_t MASASolenoid::getState(){
  return state;
}

uint8_t MASASolenoid::open(){
	if (!pinAttached || state == COOLING || state == OPEN) return -1;
	digitalWrite(controlPin,OPEN);
	timeOnStartMillis = millis();
  timeOnMillis = 0;
	state = OPEN;
	return 1;
}

uint8_t MASASolenoid::close(){
	if (state == CLOSED) return 0;
	timeOnMillis = 0;
  timeOnStartMillis = 0;
	digitalWrite(controlPin, CLOSED);
	state = CLOSED;
	
	return 1;
}

uint8_t MASASolenoid::calculateCurrent(){
	if (!currentAvailable) return 0;
	
	currentDraw = analogRead(currentPin)/4096.0 * 10.0; //Current in amps
	
	return 1;
}

uint16_t MASASolenoid::getCurrent(){
	return currentDraw;
}

uint32_t MASASolenoid::timeOn(){
	return timeOnMillis;
}

uint32_t MASASolenoid::timeCooling(){
	return timeCoolingMillis;
}

uint8_t MASASolenoid::getControlPin(){
	return controlPin;
}
uint8_t MASASolenoid::getCurrentPin(){
	return currentPin;
}
