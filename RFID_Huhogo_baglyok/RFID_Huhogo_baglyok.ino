#include <SPI.h> //RFID konyvtarak
#include <MFRC522.h>

#include "SoftwareSerial.h" //DFPlayer konyvtarak
#include "DFRobotDFPlayerMini.h"


#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); //RFID tobbi pinje nem valaszthato, de azok a 11, 12, 13 pineken vannak

#define RX_PIN 6
#define TX_PIN 7
#define BUSY_PIN 8 //Ha eppen lejatszik a DFPlayer, akkor 0, ha nem, akkor 1
SoftwareSerial softwareSerial(TX_PIN, RX_PIN);
DFRobotDFPlayerMini player;
const byte volume = 30; //Hangero 0-30 allithato


const byte DIP[4] {2, 3, 4, 5}; //DIP-Switch a 2,3,4,5 pinekre van kotve
byte DIP_val = 0;


const byte StoredID[6][4] = { {0xC3, 0x76, 0x88, 0x16}, //A 6db RFID matrica ID-je
                              {0x13, 0x07, 0x97, 0x16},
                              {0xC3, 0x9C, 0x84, 0x16},
                              {0xF6, 0x36, 0x66, 0x2D},
                              {0xA6, 0x36, 0x66, 0x2D},
                              {0xD6, 0x31, 0x66, 0x2D} };
byte ScannedID[4] = {0x00, 0x00, 0x00, 0x00}; //A modul altal szkennelt ID itt tarolodik, ID puffer



void setup()
{
  delay(1500); //Kicsi időt hagyunk, hogy az RFID inicializálás sikeres legyen

  SPI.begin(); //Serialok inditasa, inicializalas, stb.
  mfrc522.PCD_Init();
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

  player.play(1); //ezutan kevesbe zajos
}



void loop()
{

  if ( ! mfrc522.PICC_IsNewCardPresent()) //Ha nincs kartya az olvasonal, akkor az ID puffer uritese?
  {                                       //Ezt a reszt annyira nem ertem, de igy mukodik a library
    delay(10);
    ScannedID[0] = 0x00;
    ScannedID[1] = 0x00;
    ScannedID[2] = 0x00;
    ScannedID[3] = 0x00;
    return;
  }
  getID(); //ID olvasasa, lent van kifejtve

                                                                                                    //DIP-Switch ertek olvasasa es osszegzese 2 hatvanyai alapjan
  DIP_val = !digitalRead(DIP[0]) + 2*!digitalRead(DIP[1]) + 4*!digitalRead(DIP[2]) + 8*!digitalRead(DIP[3]);  //INPUT_PULLUP miatt akkor 0, ha a kapcsolo kapcsol
  if(DIP_val < 1) DIP_val = 1; //Min 1 lehet
  if(DIP_val > 9) DIP_val = 9; //Max 9 lehet
  //Serial.print("DIP_val: ");
  //Serial.println(DIP_val); //Serialon kiiratjuk


  for(byte i = 0; i < 6; i++) //ID ellenorzese, hogy valamelyik-e a 6 jo kozul
  {                           //Mivel 4 blokkbol all az ID, igy ez igy egyszerubbnek tunt, mint switch-case-el
    if(ScannedID[0] == StoredID[i][0] && ScannedID[1] == StoredID[i][1] && ScannedID[2] == StoredID[i][2] && ScannedID[3] == StoredID[i][3])
    {
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
  }

}



void getID()
{

  delay(50);

  if ( ! mfrc522.PICC_ReadCardSerial()) //Ha nincs olvasott kartya, visszaugrik a loop-ba?
  {                                     //Ezt a reszt sem igazan ertem, de mukodik a library
    return;
  }

  delay(50);

  for (byte i = 0; i < 4; i++) //Olvasott ID pufferbe tetele
  {
    ScannedID[i] = mfrc522.uid.uidByte[i];
  }

  for (byte i = 0; i < 4; i++)
  {
    Serial.print(ScannedID[i], HEX); //Serialon kiiratjuk, hasznalhato az uj ID-k megszerzesere is
    Serial.print(" ");
  }
  Serial.println();

}