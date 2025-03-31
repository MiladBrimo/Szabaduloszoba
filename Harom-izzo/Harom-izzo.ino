#include "SoftwareSerial.h" //DFPlayer konyvtarak
#include "DFRobotDFPlayerMini.h"


const byte Buttons[4] = {2, 3, 4, 5};
const byte Relays[4] = {9, 10, 11, 12};

#define RX_PIN 6
#define TX_PIN 7
#define BUSY_PIN 8 //Ha eppen lejatszik a DFPlayer, akkor 0, ha nem, akkor 1
SoftwareSerial softwareSerial(TX_PIN, RX_PIN);
DFRobotDFPlayerMini player;
const byte volume = 20; //Hangero 0-30 allithato


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
  for(int i = 0; i < 4; i++)
  {
    pinMode(Buttons[i], INPUT_PULLUP);
    pinMode(Relays[i], OUTPUT);
    digitalWrite(Relays[i], LOW);
  }

  //player.play(1); //ezutan kevesbe zajos
}



void loop()
{
  for(int i = 0; i < 4; i++)
  {
    if(!digitalRead(Buttons[i]))
    {
      player.play(i+1); // sd/mp3/0001.mp3
      delay(50); //Ennyi kesessel dolgozza fel a parancsot a DFPlayer
      digitalWrite(Relays[i], HIGH);
      
      while(!digitalRead(BUSY_PIN)) delay(50);
      
      player.pause();
      digitalWrite(Relays[i], LOW);
    }
  }

  delay(50);
}