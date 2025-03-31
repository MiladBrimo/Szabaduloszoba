#include "SoftwareSerial.h" //DFPlayer konyvtarak
#include "DFRobotDFPlayerMini.h"


#define RX_PIN 6
#define TX_PIN 7
#define BUSY_PIN 8 //Ha eppen lejatszik a DFPlayer, akkor 0, ha nem, akkor 1
SoftwareSerial softwareSerial(TX_PIN, RX_PIN);
DFRobotDFPlayerMini player;
const byte volume = 30; //Hangero 0-30 allithato


const byte DIP[4] {2, 3, 4, 5}; //DIP-Switch a 2,3,4,5 pinekre van kotve
byte DIP_val = 0;

#define REED_PIN 10


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


  for(byte i = 0; i < 4; i++) pinMode(DIP[i], INPUT_PULLUP); //Belso pullup ellenallas tokeletes, nem kell bonyolitani a gombok/kapcsolok bekoteset kulso ellenalalssal

  pinMode(BUSY_PIN, INPUT);
  pinMode(REED_PIN, INPUT_PULLUP);

  player.play(1); //ezutan kevesbe zajos
}

void loop()
{
                                                                                                    //DIP-Switch ertek olvasasa es osszegzese 2 hatvanyai alapjan
  DIP_val = !digitalRead(DIP[0]) + 2*!digitalRead(DIP[1]) + 4*!digitalRead(DIP[2]) + 8*!digitalRead(DIP[3]);  //INPUT_PULLUP miatt akkor 0, ha a kapcsolo kapcsol
  if(DIP_val < 1) DIP_val = 1; //Min 1 lehet
  if(DIP_val > 9) DIP_val = 9; //Max 9 lehet
  //Serial.print("DIP_val: ");
  //Serial.println(DIP_val); //Serialon kiiratjuk


  if(!digitalRead(REED_PIN)) //Ha valamelyik reed rele erzekel
  {
    Serial.println("Magnes erzekelve, lejatszas!");
    for(int i = 0; i < DIP_val; i++) //Annyiszor jatszuk le a huhogast, amennyi a DIp-Switch-en be van allitva
    {
      player.play(1); // sd/mp3/0001.mp3
      delay(500); //Ennyi kesessel dolgozza fel a parancsot a DFPlayer
      while(!digitalRead(BUSY_PIN)) //Megvarjuk, amig a lejatszas veget er
      {
        delay(10);
      }
    }
  }


  delay(100);

}