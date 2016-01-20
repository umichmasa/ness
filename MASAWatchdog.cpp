/*
 * MASAWatchdog.cpp
 * Initializtion of Watchdog Peripheral for SAMD21
 * Watchdog resets microcontroller if it gets frozen/stuck
 */


#include "MASAWatchdog.h"
#include "Arduino.h"
#include "sam.h"

MASAWatchdog::MASAWatchdog(){
	
}

void MASAWatchdog::init(){
  
  initializeWDTClock();

  disable();

  WDT->CONFIG.reg = WDT_CONFIG_PER(0x6); //Every 512ms

  while(WDT->STATUS.bit.SYNCBUSY);

  enable();

  
}

void MASAWatchdog::enable(){
  WDT->CTRL.reg = WDT->CTRL.reg | WDT_CTRL_ENABLE;
  while(WDT->STATUS.bit.SYNCBUSY);
}

void MASAWatchdog::disable(){
  WDT->CTRL.reg = WDT->CTRL.reg & !WDT_CTRL_ENABLE;
  while(WDT->STATUS.bit.SYNCBUSY);
}

void MASAWatchdog::clear(){
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;
  while(WDT->STATUS.bit.SYNCBUSY);
}

void MASAWatchdog::initializeWDTClock(){
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(4);
    
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_DIVSEL;
    
    while (GCLK->STATUS.bit.SYNCBUSY);  
    
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_WDT | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2;
}

void MASAWatchdog::reset(){
  GCLK->CTRL.reg = GCLK_CTRL_SWRST ;

  while ( (GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) )
  {
    /* Wait for reset to complete */
  }
}

