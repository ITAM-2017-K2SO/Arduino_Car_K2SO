#include <MotorContrl.h>
#include <UltraSchall.h>
//#include <SoftwareSerial.h>
/*
    Steuerung :
    forward -> Vorwärts                     1
    back -> Rückwärts                       2
    right -> Rechts                         3
    left -> Links                           4  
    stop -> STOP                            0
    faster -> Schneller                     8
    slow -> Langsamer                       9
 
    wenn aus folgender Perspektive betrachtet : Von hinten(Batt.Pack) nach vorne
    MOTOR_R => Rechte Seite (a)
    MOTOR_L => Linke Seite  (b)

    Bluetooth
    
    
    Änderung Oliver 25.1.2017:
    Serial1 kann jetzt mit BT abgekürzt werden
    Software Serial gegen  Serial1 ausgetauscht
    Motor_a ,lastspeed_a,lastPhase,aPhase a -> r (Motor_r), b -> l (Motor_r)
    
 */
 
//SoftwareSerial BT(10,11); //[BT-Adapter TX 10, RX 11] Änderung 

#define BT Serial1 //BT wird mit Serial1 gesetzt

const int SafeDistance = 30;

double TestPAUSE = 0;

bool FOUND_OBJ_FRONT = false, FOUND_OBJ_L = false, FOUND_OBJ_R = false;

int USensorPINS[6] = {8,9,10,11,12,13};  //Sensor FRONT: 8,9 FRONT_L: 10,11 FRONT_R: 12,13

int rPhase[2] = {6, 5};    //[MotorA]Pin rPhase des Motortreibers an Digital Pin 2,3 vom Arduino
int rEnable = 7;   //[MotorA]Pin rEnable des Motortreibers an Digital Pin 6 vom Arduino
 
int lPhase[2] = {4, 3}; //[MotorB]Pin lPhase des Motortreibers an Digital Pin 4,5 vom Arduino
int lEnable = 2; //[MotorB]Pin lEnable des Motortreibers an Digital Pin 7 vom Arduino
 
int gotten_bt;
 
int lastPhaseR = 0;
int lastPhaseL = 0;
 
int lastSpeedR = 0;
int lastSpeedL = 0;
 
boolean steht = true;
boolean vorwaerts = false;

Motor MOTOR_R(rEnable, rPhase[0], rPhase[1]);
Motor MOTOR_L(lEnable, lPhase[0], lPhase[1]);

USensor SENSOR_FRONT(USensorPINS[0],USensorPINS[1]);
USensor SENSOR_R(USensorPINS[4],USensorPINS[5]);
USensor SENSOR_L(USensorPINS[2],USensorPINS[3]);

void setup()
{
  //Bluetooth initialisieren
  BT.begin(9600);
  BT.println("Bluetooth initialized");
 
}

void Control(int bt_input)
{
  //------------------------- Set Speed --------------------------
    if(bt_input == '8')
    {
      MOTOR_R.Set_Speed(MOTOR_R.Give_Speed());
      if((MOTOR_R.Give_Speed()) <= 219 )
      {
        MOTOR_R.Set_Speed((MOTOR_R.Give_Speed() + 35));
        MOTOR_L.Set_Speed((MOTOR_L.Give_Speed() + 35));
        BT.print("Speed +35   ==>   ");BT.println(MOTOR_R.Give_Speed());
        lastSpeedR = MOTOR_R.Give_Speed();
        lastSpeedL = MOTOR_L.Give_Speed();
      }
      else
      {
	      // Set Speed auf Maximum
        MOTOR_R.Set_Speed(254);
        MOTOR_L.Set_Speed(254);
        BT.print("Speed MAX   ==>   ");BT.println(MOTOR_R.Give_Speed());
        lastSpeedR = MOTOR_R.Give_Speed();
        lastSpeedL = MOTOR_L.Give_Speed();
      }
    }
    else
    {
      if(bt_input == '9')
      {
        MOTOR_L.Set_Speed(MOTOR_R.Give_Speed());
        if( (MOTOR_R.Give_Speed()) >= 55)
        {
          MOTOR_R.Set_Speed((MOTOR_R.Give_Speed() - 35));
          MOTOR_L.Set_Speed((MOTOR_L.Give_Speed() - 35));
          BT.print("Speed -35   ==>   ");BT.println(MOTOR_R.Give_Speed());
          lastSpeedR = MOTOR_R.Give_Speed();
          lastSpeedL = MOTOR_L.Give_Speed();
        }
        else
        {
          MOTOR_R.Set_Speed(25);
          MOTOR_L.Set_Speed(25);
          BT.print("Speed MIN   ==>   ");BT.println(MOTOR_R.Give_Speed());
          lastSpeedR = MOTOR_R.Give_Speed();
          lastSpeedL = MOTOR_L.Give_Speed();
        }
      }
    }
    //--------------------------------------------------------------
    if(bt_input != '3' && bt_input != '4')
    {
      MOTOR_L.Set_Speed(MOTOR_R.Give_Speed());
    }
   
    if(bt_input == '1')
    {
      MOTOR_R.Fahren(1);
      MOTOR_L.Fahren(1);
      lastPhaseR = 1;
      lastPhaseL = 1;
      lastSpeedR = MOTOR_R.Give_Speed();
      lastSpeedL = MOTOR_L.Give_Speed();
      BT.println("Bluetooth Rückmeldung : Vorwärts");
      steht = false;
      vorwaerts = true;
    }
    else
    {
      if(bt_input == '2')
      {
        MOTOR_R.Fahren(2);
        MOTOR_L.Fahren(2);
        lastPhaseR=2;
        lastPhaseL=2;
        lastSpeedR = MOTOR_R.Give_Speed();
        lastSpeedL = MOTOR_L.Give_Speed();
        BT.println("Bluetooth Rückmeldung : Rückwärts");
        steht = false;
        vorwaerts = false;
      }
      else
      {
        if(bt_input == '3')
        {
          if(steht == true)
          {
            MOTOR_R.Fahren(2,127);//Fahren(Richtung/Geschwindigkeit)
            MOTOR_L.Fahren(1,127);
            lastPhaseR=2;
            lastPhaseL=1;
            lastSpeedR = 127;
            lastSpeedL = 127;
            BT.println("Bluetooth Rückmeldung : Rechts");
          }
          else
          {
            MOTOR_R.Fahren(lastPhaseR);
            MOTOR_L.Fahren(lastPhaseL, lastSpeedL/3);
            lastSpeedL = lastSpeedL/3;
            BT.println("Bluetooth Rückmeldung : Rechts & vorwaerts");
          }
        }
        else
        {
          if(bt_input == '4')
          {
            if(steht==true)
            {
              MOTOR_R.Fahren(1,127);
              MOTOR_L.Fahren(2,127);
              lastPhaseR=1;
              lastPhaseL=2;
              lastSpeedR = 127;
              lastSpeedL = 127;
              BT.println("Bluetooth Rückmeldung : Links");
            }
            else
            {
              MOTOR_R.Fahren(lastPhaseR, lastSpeedR/3);
              lastSpeedR = lastSpeedR/3;
              MOTOR_L.Fahren(lastPhaseL);
              BT.println("Bluetooth Rückmeldung : Links & vorwaerts");
            }
          }
          else
          {
            if(bt_input == '0')
            {
              MOTOR_R.Fahren(0);
              MOTOR_L.Fahren(0);
              lastPhaseR = 1;
              lastPhaseL = 1;
              lastSpeedR = 0;
              lastSpeedL = 0;
              steht = true;
              BT.println("Bluetooth Rückmeldung : HALT STOP !");
            }
          }
        }
      }
    }
}

void Notbremse()
{
  double pause = 0;
  pause = millis() + 400;
  Control('2');
  do{/* Platzhalter */}while(pause>millis());
  Control('0');
  Control('4');
  pause = millis() + 2500;
  do{/* Platzhalter */}while(pause>millis());
  Control('0');
}

void Ausweichen(bool FRONT, bool LEFT, bool RIGHT)
{
  if(FRONT && LEFT && RIGHT)
  {
    Notbremse();
  }else if(FRONT && (!LEFT) && (!RIGHT))
        {
          bool AusweichCheck = true;
          double timeBegin = NULL;
          Control('4');
          Control('4');
          timeBegin = millis();
          do
          {
            AusweichCheck = SENSOR_FRONT.CheckForObjects(SafeDistance);
          }while(AusweichCheck);
          double timeEnd = millis();
          double durationNeeded = timeEnd - timeBegin;
          double nxt = millis()+durationNeeded;
          Control('3');
          Control('3');
          do{BT.write("Ich haenge in der schleife(1.1)");}while(millis() < nxt);
          BT.write("Verlassen(1.1)");
          Control('4');
          Control('4');
          nxt = nxt + 500;
          BT.write((int)nxt);
          do{BT.write("Ich haenge in der schleife(1.2)");}while(millis() < nxt);
          BT.write("Verlassen(1.2)");
          Control('1');
        } else if((!FRONT)&& LEFT && (!RIGHT))
               {
                   bool AusweichCheck = true;
                  double timeBegin = NULL;
                  Control('3');
                  Control('3');
                  timeBegin = millis();
                  do
                  {
                    AusweichCheck = SENSOR_L.CheckForObjects(SafeDistance+10);
                  }while(AusweichCheck);
                  double timeEnd = millis();
                  double durationNeeded = timeEnd - timeBegin;
                  double nxt2 = millis()+durationNeeded;
                  Control('4');
                  Control('4');
                  do{BT.write("Ich haenge in der schleife(2.1)");}while(millis() < nxt2);
                  BT.write("Verlassen(2.1)");
                  Control('3');
                  Control('3');
                  nxt2 = millis() + 500;
                  BT.write((int)nxt2);
                  do{BT.write("Ich haenge in der schleife(2.2)");}while(millis() < nxt2);
                  BT.write("Verlassen(2.2)");
                  Control('1');
               } else if((!FRONT)&& (!LEFT) && RIGHT)
                      {
                        
                        bool AusweichCheck = true;
                        double timeBegin = NULL;
                        Control('4');
                        Control('4');
                        timeBegin = millis();
                        do
                        {
                          AusweichCheck = SENSOR_R.CheckForObjects(SafeDistance+10);
                        }while(AusweichCheck);
                        double timeEnd = millis();
                        double durationNeeded = timeEnd - timeBegin;
                        double nxt3 = millis()+durationNeeded;
                        Control('3');
                        Control('3');
                        do{BT.write("Ich haenge in der schleife(3.1)");}while(millis() < nxt3);
                        BT.write("Verlassen (3.1)");
                        Control('4');
                        Control('4');
                        nxt3 = millis() + 500;
                        BT.write((int)nxt3);
                        do{BT.write("Ich haenge in der schleife(3.2)");}while(millis() < nxt3);
                        BT.write("Verlassen (3.2)");
                        Control('1');
                      };
}
void loop()
{
  if(BT.available())
  {
    gotten_bt = BT.read();
    BT.println("Bluetooth Rückmeldung : Hat folgenden Wert empfangen: ");
    BT.println(gotten_bt);
    Control(gotten_bt); //Fahren
  }
    if(!steht && vorwaerts)
    {
       if(TestPAUSE == 0)
       {
          TestPAUSE = millis() + 10;
       };
       
       if(millis() >= TestPAUSE)
       {
         FOUND_OBJ_FRONT = SENSOR_FRONT.CheckForObjects(SafeDistance);
         FOUND_OBJ_R = SENSOR_R.CheckForObjects(SafeDistance+10);
         FOUND_OBJ_L = SENSOR_L.CheckForObjects(SafeDistance+10);
//         float testEntfernung = SENSOR_FRONT.SonarPingDistance();
//         BT.print("Entfernung: ");
//         BT.println(testEntfernung);
         Ausweichen(FOUND_OBJ_FRONT, FOUND_OBJ_R, FOUND_OBJ_L);
         TestPAUSE = millis() + 10;
       };
    };
}
