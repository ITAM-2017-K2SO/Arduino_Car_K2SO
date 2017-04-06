/*
	
	UltraSchall.h    -  Library für die U-Sensoren beim Arduino Projekt

	letzte Aenderung durch :   Louis        , am :     01.02.'17
	
*/

#ifndef UltraSchall_h
#define UltraSchall_h

#include "Arduino.h"

class USensor
{

	protected:
		
		int TriggerPIN;		//Trigger PIN
		int EchoPIN;		//Echo PIN

	public:

		USensor(int gotTriggerPIN, int gotEchoPIN);			//Konstruktor der Klasse USensor
		float SonarPing();									//Einen Suchlauf triggern
		float PingToDistance(float givenPing);				//Einen SensorPing zu einer Entfernung umrechnen
		float SonarPingDistance();							//Einen Suchlauf triggern und Entfernunf zurückgeben
		bool CheckForObjects(float secureDistance = 2.5);	//Überprüfen ob ein Objekt im Weg ist
};

#endif