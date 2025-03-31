#include "SoftwareSerial.h" //DFPlayer konyvtarak
#include "DFRobotDFPlayerMini.h"


#define RX_PIN 6
#define TX_PIN 7
#define BUSY_PIN 8 //Ha eppen lejatszik a DFPlayer, akkor 0, ha nem, akkor 1
SoftwareSerial softwareSerial(TX_PIN, RX_PIN);
DFRobotDFPlayerMini player;

byte volume = 30; //Hangero 0-30 allithato
#define Pot A0



void setup()
{
  delay(1500); //Kicsi időt hagyunk, hogy az inicializálás sikeres legyen

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


  player.loop(1);
}



void loop()
{
  volume = map(analogRead(Pot), 10, 1000, 0, 30);
  player.volume(volume);

  delay(100);
}