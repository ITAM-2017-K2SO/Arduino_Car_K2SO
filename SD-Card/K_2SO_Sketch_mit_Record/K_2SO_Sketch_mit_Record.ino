#include <SD.h>

//#include <SoftwareSerial.h>
/*
     Steuerung :
    forward -> Vorwärts soll noch (1)
    back -> Rückwärts             (2)
    right -> Rechts               (3)
    left -> Links                 (4)
    stop -> STOP                  (0)
    faster -> Schneller           (8)
    slow -> Langsamer             (9)
    5,6,7 frei
    1 -> 49
    5 -> 53
    
    97 -> a
    98 -> b
    
    !!!!Meine SD-CARD muss auf 5V laufen!!!!
    5V  5V
    GND  GND
    MOSI  51
    MISO  50
    SCK  52
    CS   53
    
    
    wenn aus folgender Perspektive betrachtet : Von hinten(Batt.Pack) nach vorne
    MOTOR_R => Rechte Seite (a)
    MOTOR_L => Linke Seite  (b)

    Bluetooth
    
    
    Änderung Oliver 25.1.2017:
    Serial1 kann jetzt mit BT abgekürzt werden
    Software Serial gegen  Serial1 ausgetauscht
    Motor_a ,lastspeed_a,lastPhase,aPhase a -> r (Motor_r), b -> l (Motor_r)
    
     Änderung Oliver 01.02.2017:
    Hinzufügen von der Funktion void record(); Die Zuständig ist die Bluetooth-Befehle auf 
    SD Karte zu loggen
    
 */
 
//SoftwareSerial BT(10,11); //[BT-Adapter TX 10, RX 11] Änderung 

File myFile;

boolean record = false;

int pinCs = 53; // SD-card




int gotten_bt;
boolean steht = true;
 

void Route_record(int number)
{
  if(record == true)
  {
    myFile = SD.open("record.txt", FILE_WRITE);
    if(myFile)
    {
      if (number != 97)
      {
      myFile.println(number);
      //myFile.println(number);
      //myFile.print(",");
      myFile.close();
      Serial.println(number);
      }   
    }else
    {
    Serial.println("error opening record.txt");
    }
  }
}

void Route_play()
{
  myFile = SD.open("record.txt");
  if(myFile)
  {
    Serial.println("Play Route...");
    while (myFile.available()) {
       int input_log = myFile.read();
       if(input_log != 44 || input_log != 10 || input_log != 13 || input_log != '\0')
       {  
       Serial.println(input_log);
       Control(input_log);
       }
    }
  }
   myFile.close();
}

void setup()
{
  //Bluetooth initialisieren
  Serial.begin(9600);

  Serial.println("Bluetooth initialized");
  pinMode(pinCs, OUTPUT);
  // Test bt_input == 7;
  if(SD.begin(pinCs))
  {
    Serial.println("SD card is ready to use.");
  }else
  {

  Serial.println("SD card initialized failed");
  }
   //TEST bt_input == 5;
   //record();
}

void Control(int bt_input)
{
    if(bt_input == '8')
    {
        Serial.print("8 wird ausgeführt."); 
    }
    if(bt_input == '9')
    {
      Serial.println("9 wird ausgeführt.");
    }

   
    if(bt_input == '1')
    {
      Serial.println("1 wird ausgeführt.");
    }
    if(bt_input == '2')
      {
        Serial.println("2 wird ausgeführt");
      }
        if(bt_input == '3')
        {
            Serial.println("3 wird ausgeführt.");
        }

          if(bt_input == '4')
          {
           Serial.println("4 wird ausgeführt.");
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
  pause = millis() + 1350;
  do{/* Platzhalter */}while(pause>millis());
  Control('0');
}

void loop()
{
  if(Serial.available())
  {
    gotten_bt = Serial.read();
    Serial.println("Serial Rückmeldung : Hat folgenden Wert empfangen: ");
    Serial.println(gotten_bt);
    
    if(gotten_bt == 97)
    {
     if(record == false)
     {
      record = true; 
      Serial.println("record auf true .");
     }else
     {
      record = false; 
      Serial.println("record auf false. ");
     }  
    }
    if (record == true)
     {
      Route_record(gotten_bt); 
     }  
    if(gotten_bt == 98)
    {
      Route_play();
    }
    

    Control(gotten_bt); //Fahrens
  }
    
}
