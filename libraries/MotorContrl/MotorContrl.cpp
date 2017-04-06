/*

	MotorContrl.cpp   -  Motor Library Cpp-File

	letzte Aenderung durch :   Louis        , am :      25.02.'16

*/
#include "Arduino.h"
#include "MotorContrl.h"


Motor::Motor(int got_Enable, int got_Phase1, int got_Phase2)
{//Übergabe = got_Enable, got_Phase[2]
	EnablePIN = got_Enable;
	PhasePIN[0] = got_Phase1;
	PhasePIN[1] = got_Phase2;
	Speed = 255;

	pinMode(EnablePIN, OUTPUT);
	pinMode(PhasePIN[0], OUTPUT);
	pinMode(PhasePIN[1], OUTPUT);
}

int Motor::Give_Speed()
{
	return Speed;
}

void Motor::Set_Speed(int givenSpeed)
{
	Speed = givenSpeed;
}

void Motor::Fahren(int Richtung, int givenSpeed)
{
	if(givenSpeed == 0)
	{
		givenSpeed = Speed;
	}

	switch(Richtung)
	{
	case 1:
		digitalWrite(PhasePIN[1], LOW);
		digitalWrite(PhasePIN[0], HIGH);
		analogWrite(EnablePIN, givenSpeed);

		break;

	case 2:
		digitalWrite(PhasePIN[1], HIGH);
		digitalWrite(PhasePIN[0], LOW);
		analogWrite(EnablePIN, givenSpeed);
		break;

	case 0:
		digitalWrite(PhasePIN[1], LOW);
		digitalWrite(PhasePIN[0], HIGH);
		analogWrite(EnablePIN, 0);
		break;
	}
}