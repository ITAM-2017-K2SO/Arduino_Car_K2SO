/*
	
	UltraSchall.cpp    - U-Sensoren Library Cpp-File

	letzte Aenderung durch :   Louis        , am :      01.02.'17
	
*/
#include "Arduino.h"
#include "UltraSchall.h"

USensor::USensor(int gotTriggerPIN, int gotEchoPIN)
{
	TriggerPIN = gotTriggerPIN;
	EchoPIN = gotEchoPIN;

	pinMode(TriggerPIN, OUTPUT);
	pinMode(EchoPIN, INPUT);

	digitalWrite(TriggerPIN, LOW);
}

float USensor::SonarPing()
{
	float ergebnis = 0;
	int now = micros();
	bool fertig = false;

	digitalWrite(TriggerPIN, HIGH);
	do
	{
		if(now >= now+10)
		{
			digitalWrite(TriggerPIN, LOW);
			ergebnis = pulseIn(EchoPIN, HIGH);
			fertig = true;
		}
	}while(!fertig);

	return ergebnis;
}

float USensor::PingToDistance(float givenPing)
{
	float ergebnis = 0;

	ergebnis = (givenPing/2)/29.1;

	return ergebnis;
}

float USensor::SonarPingDistance()
{
	float ping = 0;
	ping = SonarPing();
	float ergebnis = PingToDistance(ping);

	return ergebnis;
}

bool USensor::CheckForObjects(float secureDistance)
{
	bool ergebnis = false;
	int milli_Now = millis();
	
	float distance = SonarPingDistance();

	if(distance <= secureDistance)
	{
		ergebnis = true;
	}
	
	return ergebnis;
}