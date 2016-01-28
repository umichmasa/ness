/*
 * MASAWatchdog.cpp
 * Initializtion of Watchdog Peripheral for SAMD21
 * Watchdog resets microcontroller if it gets frozen/stuck
 */


//Initialization for a reset watchdog
//Resets program if no response after a given time
//Prevents program from locking up forever




#ifndef MASAWatchdog_h
#define MASAWatchdog_h


#include <inttypes.h>
#include <stdlib.h>
#include <Arduino.h>
#define VERSION_NUMBER 1 //Original Release


//Main Class
class MASAWatchdog{

  public:
  //Constructor
  MASAWatchdog();

  //Initialize (set registers)
  void init();

  //Enable (turn checking register on)
  void enable();

  //Disable (turn checking register off)
  void disable();

  //Must be called periodically to clear timer
  void clear();

  //Borrowed from internet, sets up clock
  void initializeWDTClock();

  //Software
  void reset();
  
};


#endif
