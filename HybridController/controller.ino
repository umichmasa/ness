/////////////////////////////
//defines
/////////////////////////////

#define CONNECTION_MILLIS 500

//Pins!!
#define PRESSURE_PIN A0
#define VENT_PIN 9
#define VENT_CURR_PIN A3
#define DANGER_PRESSURE 3105

/////////////////////////////
//global vars
/////////////////////////////


uint16_t lastConnectionCheck = 0;



void setup()
{
	pinMode(VENT_PIN, OUTPUT); //attach pin to vent solenoid

}

void loop()
{
	if((millis() - lastConnectionCheck) >= CONNECTION_MILLIS)
	{
		checkConnection();
		lastConnectionCheck = millis();
	}
	uint16_t pressure = anologRead(PRESSURE_PIN);
	if(checkEmergency(pressure))
	{
		emergencyState();
	}
	else
	{
		normalState();
	}
	


}

checkConnection()
{
	

}
bool checkEmergency(uint16_t pressure)
{
	return pressure > DANGER_PRESSURE;
}
emergencyState()
{
	digitalWrite(VENT_PIN, HIGH);
}


