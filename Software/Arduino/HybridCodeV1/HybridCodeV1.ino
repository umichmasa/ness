/*
 * Main file for Hybrid Controller V1 Code
 * Code for V1 of the MASA Custom PCB Hybrid Controller
 * Compile with Arduino/Genuino Zero Board Definitions
 * Object Oriented Structure
 * Round Robin on Timers with Interrupts Architecture
 * 1/02/2015
 * 
 * TODO
 * -Complete Config
 * -Begin Datalogging
 * -Investigate FreeRTOS implementation for future task scheduling
 * -Improve usage of timimg from solenoids/servo
 * -Figure out why timing for MASAServo is strange?
 * -Define actual open procedure for servo
 * -Define beter interval for polling ADC
 */

 //Arduino Libraries
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>

//SAMD21/Arduino Zero Libraries
#include <RTCZero.h>

#include "sam.h"
#include "samd.h"

//Custom Libraries
#include "MASASerial.h"
#include "MASAServo.h"
#include "MASASolenoid.h"
#include "MASAWatchdog.h"
#include "MASAEEPROM.h"
#include "MASAFlash.h"

#include "MASAConfig.h"

#include "hybrid_pins.h"
#include "hybrid_constants.h"

//Load from EEPROM
uint8_t servoOpen = SERVO_CLOSED;
uint8_t servoClosed = SERVO_OPEN;
uint8_t sendTime = SEND_TIME;
uint8_t receiveTime = RECEIVE_TIME;

uint16_t overPressure = OVER_PRESSURE;







//Time Ignition starts
uint32_t ignitionStart = 0;


//Custom Objects
MASASerial serial;
MASAServo mov;
MASASolenoid fill;
MASASolenoid vent;
MASAWatchdog wdt;
//MASAEEPROM eeprom;
MASAFlash flash;

MASAConfig config;

RTCZero rtc;

//Timing
uint32_t lastMessage = 0;
uint32_t lastSend = 0;
uint32_t lastReceive = 0;

//Controller States
uint8_t connected = 0;

//Output States
uint8_t fillState = 0;
uint8_t ventState = 0;
uint8_t servoAngle = 0;

//Input States
uint16_t pressure = 0;
uint16_t pressureSamples[ANALOG_SAMPLES];
uint16_t pressureAverage = 0;
uint8_t fuseState = FUSE_CLOSED;
uint8_t ignitionState = 0;
uint8_t unlockState = 0;
uint8_t overrideState = 0;

uint8_t emergencyState = 0;


//From MATLAB
uint8_t matServoAngle=0;
uint8_t matFillState=0;
uint8_t matVentState=0;
uint8_t matOverrideState=0;
uint8_t matUnlockState=0;

//Called on boot
void setup() {
  //Instantiate custom objects
  mov.attach(SERVO_S_PIN);
  mov.writeAngle(SERVO_CLOSED);
  fill.setControlPin(FILL_EN_PIN);
  vent.setControlPin(VENT_EN_PIN);
  config.init();
  
  //Communicate with Arduino Uno software serial for ASCII debugging
  Serial1.begin(4800);

  //Servo enable logic
  pinMode(SERVO_EN_PIN,OUTPUT);
  digitalWrite(SERVO_EN_PIN,LOW);
  pinMode(FUSE_S_PIN,INPUT);

  //Tx/Rx LED Setup
  pinMode(PIN_LED_RXL,OUTPUT);
  pinMode(PIN_LED_TXL,OUTPUT);
  digitalWrite(PIN_LED_RXL,HIGH);
  digitalWrite(PIN_LED_TXL,HIGH);


  digitalWrite(PIN_LED_RXL,LOW);
  
  //Read config values
  //readGeneralConfig();
  digitalWrite(PIN_LED_RXL,HIGH);
  //Define servo open procedure
  establishServoProcedure();

  //Increase resolution of analog read from default 10 bits
  analogReadResolution(12);

  //Instantiate watchdog timer
  wdt.init();

}

//Called as fast as possible
void loop() {

  //Get current time
  uint32_t currTime = millis();

  //If not connected, try to connect
  if(!connected){
    checkConnection();
  }

  //Send message if connected on timed loop
  if (connected==1 && (currTime - lastSend) >= SEND_TIME){
    sendSerialStateOutput();
    //SerialUSB.println(currTime);
    lastSend = currTime;
  }
  //Check for message if connected on timed loop
  if (connected==1 && (currTime - lastReceive) >= RECEIVE_TIME){
    checkSerialInput();
    lastReceive = currTime;
  }

  //Update serial object
  serial.update();

  //Read pressure, recalculate average
  updatePressure();
  updateAveragePressure();

  //Check for emergency pressure
  checkEmergency();

  //Check if fuse ignited
  checkFuseBlow();

  //Update actuator objects
  mov.update(currTime);
  fill.update(currTime);
  vent.update(currTime);


  //Handle Emergency Condition
  if (emergencyState&&!overrideState){
    handleEmergency();
  }

  //Handle Ignition Conditions
  else if ((fuseState==FUSE_OPEN) && matUnlockState){
    handleIgnition();
  }

  //Handle Normal Conditions
  else {
    handleNormal();
  }

  //MUST BE CALLED WITHIN 512ms OR BOARD REBOOTS
  wdt.clear();

  
}

//Reads configuation settings from EEPROM
void readGeneralConfig(){
  

  servoClosed = config.readGeneralConfigurationByte(0);
  servoOpen = config.readGeneralConfigurationByte(1);
  overPressure = config.readGeneralConfigurationByte(2)<<8 + config.readGeneralConfigurationByte(3);
}

//Define servo open procedure
void establishServoProcedure(){
/*
1) Wait 1.5 seconds
2) Open 10% (about 9 degrees)
3) Wait 0.5 seconds
4) Open to 50% (about 45 degrees) at rate of (22.5 deg/s)
5) Open to 100% (90 degrees or fully open)
*/
  /*  
   *   struct MASAServoCommand{
          uint8_t angle;
          uint16_t time;
          uint16_t velocity;
        };
   */

  mov.addCommand(SERVO_CLOSED, 1);
  mov.addCommand(0, 1500);
  mov.addCommand(8, 1000);
  mov.addCommand(20, 1000);
  mov.addCommand(10,1000);
  mov.addCommand(70,1000);




  
}



//Checks energency pressure condition
void checkEmergency(){
  if (pressureAverage>=overPressure){
    emergencyState=1;
  }else{
    emergencyState=0;
  }
}

//Checks if fuse opens
//Works like software latch
void checkFuseBlow(){
  
  if ((fuseState==FUSE_CLOSED)&&digitalRead(FUSE_S_PIN)==FUSE_OPEN){
    ignitionState = 1;
    fuseState = FUSE_OPEN;
  }
  
  
}

//Process emergency conditions
void handleEmergency(){
  vent.open();
}

//Process normal conditions when connected
void handleNormal(){
  if (connected){

    if (matFillState){
      fill.open();
    }else{
      fill.close();
    }

    if (matVentState){
      vent.open();
    }else{
      vent.close();
    }
    digitalWrite(SERVO_EN_PIN,matUnlockState);
    mov.writeAngle(matServoAngle);
  }
  
}

//Processes ignition conditions
void handleIgnition(){
    //digitalWrite(SERVO_EN_PIN,HIGH);
    if (ignitionStart == 0) ignitionStart = millis();
    if(!mov.commandStarted()) mov.startCommand();
    servoAngle = mov.getOutput();
    
}

//Reads in pressure data
void updatePressure(){
  static uint8_t i;
  
  pressure = map(analogRead(PRESSURE_S_PIN),0,4096,0,1320);
  pressureSamples[i] = pressure;
  i++;
  i=i%ANALOG_SAMPLES;
}

//Calculates more stable average pressure
void updateAveragePressure(){
  pressureAverage = 0;
  uint32_t counter = 0;
  for (uint8_t i = 0; i < ANALOG_SAMPLES; i++){
    counter+=pressureSamples[i];
  }
  pressureAverage = counter/(float)ANALOG_SAMPLES;
  
}

//Determines if connection message has been received from MATLAB
void checkConnection(){
  //Serial1.println("Checked Connection");

  //Connection message received
  if (serial.messageAvailable()&&serial.getMessageHeader()==serial.SHOUT){
    //SerialUSB.println("found");
    uint8_t* data = serial.getMessage();
    size_t length = serial.getMessageLength();

    const uint8_t EXPECTED_LENGTH = 6;
    
    uint8_t id = data[0];
    //uint8_t version = data[1];
    //uint8_t commMethod = data[2];

    //digitalWrite(PIN_LED_RXL,LOW);
    
    if (length == EXPECTED_LENGTH && data[0]=='M'){
      digitalWrite(PIN_LED_TXL,LOW);
      uint8_t version = data[1];
      uint8_t commMethod = data[2];
      connected=true;
      uint8_t out[3] = {'H',1,'U'};
      for (int i = 0; i < 10; i++){
      serial.sendData(serial.SHOUT, out, 3);
        delay(25);
      }

      rtc.begin();
      rtc.setSeconds(data[3]);
      rtc.setMinutes(data[4]);
      rtc.setHours(data[5]);
      
    }

   //Configuation message received
  }else if (serial.messageAvailable()&&serial.getMessageHeader()==serial.CONFIG){
      Serial1.println("Found Config");
      
      digitalWrite(PIN_LED_RXL,LOW);
      uint8_t* data = serial.getMessage();
      size_t length = serial.getMessageLength();
      const uint8_t EXPECTED_LENGTH = 4;
      if (length == EXPECTED_LENGTH && data[0] == 'M'){
        uint8_t action = data[1];
        uint8_t incomingBytesHigher = data[2];
        uint8_t incomingBytesLower = data[3];
        uint8_t out[3] = {'H',1,'U'};
        //for (int i = 0; i < 3; i++){
          serial.sendData(serial.SHOUT, out, 3);
        //  delay(25);
        //}

        Serial1.println("read in");
        Serial1.println(data[2]);
        Serial1.println(data[3]);

        uint16_t incomingBytes = incomingBytesHigher << 8;
        incomingBytes+=incomingBytesLower;

        Serial1.println(incomingBytes);
        
        writeConfig(action,incomingBytes, data, length);
      }
      
  }
}

//Collect config settings from MATLAB
//Implementation in progress
void writeConfig(uint8_t action, uint16_t incomingBytes, uint8_t* data, size_t length){
  Serial1.println("Writing Config");
  Serial1.println(action);
  Serial1.println(incomingBytes);
  if (action == config.GENERAL_CONFIG){
    Serial1.println("Found Action");
    digitalWrite(PIN_LED_TXL,LOW);
    uint16_t bytesReceived = 0;
    //uint8_t datas[1] = {1};
    //uint8_t* data = datas;
    //size_t length = 0;
    while(bytesReceived < incomingBytes){
      delay(100);
      Serial1.print("Found byte "); Serial1.println(bytesReceived);
      Serial1.print(serial.messageAvailable());Serial1.print(" "); Serial1.println(serial.getMessageHeader());
      if (serial.messageAvailable()&&serial.getMessageHeader()==serial.CONFIG){
        Serial1.println("Got message");
        data = serial.getMessage();
        length = serial.getMessageLength();
        config.writeGeneralConfigurationByte(data[2],data[1]);

        
        
        bytesReceived++;
      }

      serial.sendData(serial.CONFIG_RESPONSE,data,length);

      wdt.clear();
    }
    

    
  }else if (action == config.SERVO_CONFIG){
    
  }
  Serial1.println("Leaving config");
  wdt.reset();
  
}

//Writes state message to MATLAB
void sendSerialStateOutput(){
  const uint8_t DATA_LENGTH = 29;
  uint8_t data[DATA_LENGTH];
  data[0] = 'H';
  data[1] = 'S';
  data[2] = matFillState;
  data[3] = matVentState;
  data[4] = mov.getOutput();
  //memcpy(data+5,&pressure,sizeof(uint16_t));
  data[5] = pressure>>8;
  data[6] = pressure;
  
  //memcpy(data+7,&pressureAverage,sizeof(uint16_t));
  data[7] = pressureAverage>>8;
  data[8] = pressureAverage;
  
  data[9] =  rtc.getSeconds();
  data[10] = rtc.getMinutes();
  data[11] = rtc.getHours();
  data[12] = matOverrideState;
  data[13] = emergencyState;
  data[14] = ignitionState;
  data[15] = fuseState;
  data[16] = matUnlockState;
  data[17] = fill.getState();
  data[18] = vent.getState();
  //memcpy(data+19,&ignitionStart,sizeof(uint16_t));
  data[19] = ignitionStart>>8;
  data[20] = ignitionStart;

  data[21] = fill.timeOn()>>8;
  data[22] = fill.timeOn();
  data[23] = vent.timeOn()>>8;
  data[24] = vent.timeOn();

  data[25] = fill.timeCooling()>>8;
  data[26] = fill.timeCooling();
  data[27] = vent.timeCooling()>>8;
  data[28] = vent.timeCooling();

  
  serial.sendData(serial.SHOUT, data, DATA_LENGTH);

  
}

//Attempts to read state/commands from MATLAB
void checkSerialInput(){
  const uint8_t DATA_LENGTH = 6;
  if (serial.messageAvailable()){
    //digitalWrite(PIN_LED_RXL,LOW);
  uint8_t* data = serial.getMessage();
  size_t length = serial.getMessageLength();

  if (length ==DATA_LENGTH && data[0] == 'M'){
    
  
  matServoAngle=data[1];
  matFillState=data[2];
  matVentState=data[3];
  matOverrideState=data[4];
  matUnlockState=data[5];

  }

  }
  
}



