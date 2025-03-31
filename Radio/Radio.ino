#include "SoftwareSerial.h" //DFPlayer konyvtarak
#include "DFRobotDFPlayerMini.h"


#define RX_PIN 6
#define TX_PIN 7
#define BUSY_PIN 8 //Ha eppen lejatszik a DFPlayer, akkor 0, ha nem, akkor 1
SoftwareSerial softwareSerial(TX_PIN, RX_PIN);
DFRobotDFPlayerMini player;
const byte volume = 20; //Hangero 0-30 allithato


#define Pot A0
#define Led 9

byte State = 0;
byte last_State = 0;

bool Flicker = false;



void setup()
{
  softwareSerial.begin(9600);
  Serial.begin(9600);


  if (player.begin(softwareSerial)) //Serial visszajelzes az mp3 kommunikaciojarol
  {
    Serial.println(F("Connecting to DFPlayer: OK"));
    player.volume(volume);
  }
  else
  {
    Serial.println(F("Connecting to DFPlayer: Failed!"));
  }


  pinMode(BUSY_PIN, INPUT);
  pinMode(Pot, INPUT);
  pinMode(Led, OUTPUT);
  digitalWrite(Led, LOW);

  randomSeed(analogRead(0));
}



void loop()
{
  switch(analogRead(Pot))
  {
    case 330 ... 350:
      State = 1;
    break;

    case 500 ... 520:
      State = 2;
    break;

    case 670 ... 690:
      State = 3;
    break;

    default:
      State = 0;
    break;
  }
  
  
  if(State != last_State)
  {
    last_State = State;
    
    switch(State)
    {
      case 0:
        player.pause();
        Flicker = false;
      break;

      case 1:
        player.loop(1);
        Flicker = false;
      break;


      case 2:
        player.loop(2);
        Flicker = false;
      break;


      case 3:
        player.loop(3);
        delay(500);
        Flicker = true;
      break;
    }
  }


  if(Flicker)
  {
    analogWrite(Led, random(10, 150));
    delay(random(30, 130));
  }
  else
  {
    digitalWrite(Led, LOW);
  }

  delay(20);
}