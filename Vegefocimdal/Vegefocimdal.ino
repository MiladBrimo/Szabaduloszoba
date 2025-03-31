#include "SoftwareSerial.h" //DFPlayer konyvtarak
#include "DFRobotDFPlayerMini.h"


#define Trigger 2
#define Relay 9

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
  pinMode(Trigger, INPUT);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);

  //player.play(1); //ezutan kevesbe zajos
}



void loop()
{
  if(digitalRead(Trigger))
  {
    player.play(1); // sd/mp3/0001.mp3
    delay(500); //Ennyi kesessel dolgozza fel a parancsot a DFPlayer
    digitalWrite(Relay, HIGH);
     
    while(!digitalRead(BUSY_PIN)) delay(50);
      
    player.pause();
    digitalWrite(Relay, LOW);
  }

  delay(50);
}