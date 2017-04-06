/*

	MotorContrl.h    -  Library für die Motoren beim Arduino Projekt

	letzte Aenderung durch :   Louis        , am :      25.02.'16

*/

#ifndef MotorContrl_h
#define MotorContrl_h

#include "Arduino.h"


class Motor
{
	protected:
		
		int EnablePIN;  
		int PhasePIN[2];
		int Speed;

	public:

		Motor(int got_Enable, int got_Phase1, int got_Phase2);
		void Fahren(int Richtung, int givenSpeed = 0);
		void Set_Speed(int given_Speed);
		int Give_Speed();
};

#endif