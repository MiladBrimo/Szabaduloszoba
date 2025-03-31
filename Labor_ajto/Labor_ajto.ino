#include <SPI.h>
#include <MFRC522.h>


#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define Buzzer 2
#define Red 3
#define Green 4
#define Relay 5


byte ScannedID[4] = {0x00, 0x00, 0x00, 0x00};

const byte AccessCard[3][4] = //Szekesfehervar
  {
    {0x23, 0x4D, 0x32, 0xDA},
    {0xF3, 0x26, 0x31, 0xDA},
    {0x63, 0xE4, 0x1D, 0xDA}
  };
/*const byte AccessCard[3][4] = //Pecs
  {
    {0x03, 0x24, 0xD6, 0xD9},
    {0x33, 0x74, 0x15, 0xDA},
    {0x63, 0x26, 0xCF, 0xD9}
  };*/

const int blinkDelay = 200;
const int accessDelay = 3000;
const int deniedDelay = 2000;

bool match = false;



void setup()
{
	SPI.begin();
  mfrc522.PCD_Init();

  Serial.begin(9600);


  pinMode(Buzzer, OUTPUT);  digitalWrite(Buzzer, LOW);
  pinMode(Red, OUTPUT);     digitalWrite(Red, LOW);
  pinMode(Green, OUTPUT);   digitalWrite(Green, LOW);
  pinMode(Relay, OUTPUT);    digitalWrite(Relay, LOW);
}



void loop() 
{
  mfrc522.PCD_Init();

  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    delay(10);
    for(int i = 0; i < 4; i++) ScannedID[i] = 0x00;
    return;
  }


  getID();

  checkCard();

  isMatch();

}



void getID()
{

  delay(50);

  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  delay(50);

  for (byte i = 0; i < 4; i++)
  {
    ScannedID[i] = mfrc522.uid.uidByte[i];
  }

  for (byte i = 0; i < 4; i++)
  {
    Serial.print(ScannedID[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

}



void checkCard()
{

  for(int i = 0; i < 3; i++)
  {
    if(ScannedID[0] == AccessCard[i][0] && ScannedID[1] == AccessCard[i][1] && ScannedID[2] == AccessCard[i][2] && ScannedID[3] == AccessCard[i][3]) match = true;
  }

}



void isMatch()
{

  if(match)
  {
    Serial.println(F("Authorized access!"));

    for(int i = 0; i < 4; i++) ScannedID[i] = 0x00;

    digitalWrite(Green, HIGH);
    digitalWrite(Relay, HIGH);
    digitalWrite(Buzzer, HIGH);
    delay(accessDelay);
    digitalWrite(Green, LOW);
    digitalWrite(Red, LOW);
    digitalWrite(Relay, LOW);
    digitalWrite(Buzzer, LOW);
    match = false;
  }
  else
  {
  Serial.println(F("Access denied!"));

  for(int i = 0; i < 4; i++) ScannedID[i] = 0x00;

  digitalWrite(Red, HIGH);
  for(int i = 0; i < 3; i++)
  {
    digitalWrite(Buzzer, HIGH);
    delay(50);
    digitalWrite(Buzzer, LOW);
    delay(250);
  }
  delay(deniedDelay);
  digitalWrite(Red, LOW);
  digitalWrite(Green, LOW);
  }

}