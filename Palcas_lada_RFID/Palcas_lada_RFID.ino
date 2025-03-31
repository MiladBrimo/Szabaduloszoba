#include <Wire.h>

#include <SPI.h>
#include <MFRC522.h>

#include <String.h>

#define RST_PIN 2
#define SS_1_PIN 3
#define SS_2_PIN 4
#define SS_3_PIN 5
#define SS_4_PIN 6 
#define SS_5_PIN 7

#define NR_OF_READERS   5

byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN, SS_5_PIN};
MFRC522 mfrc522[NR_OF_READERS];

String card_ids[NR_OF_READERS] = {"64310245", "704310245", "1344310245", "2144910245", "2465410245"};

bool palcak[8] = {1, 1, 1, 1, 1, 1, 1, 1};


void setup()
{
	Serial.begin(9600); // Setup Serial Monitor
  Wire.begin(); // join I2C bus as the master
  SPI.begin(); // Init SPI bus

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print("Reader ");
    Serial.print(reader+1);
    Serial.print(": ");
    mfrc522[reader].PCD_DumpVersionToSerial();
    delay(50);
  }
}


void loop()
{
  Serial.println("");

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    // Look for new cards
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print("Reader ");
    Serial.print(reader+1);

    if (mfrc522[reader].PICC_IsNewCardPresent()&& mfrc522[reader].PICC_ReadCardSerial())
    {
      Serial.print(": Card UID:");
      byte cardnum;
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size, cardnum);

      mfrc522[reader].PICC_HaltA(); // Halt PICC
      mfrc522[reader].PCD_StopCrypto1(); // Stop encryption on PCD

      if(cardnum) palcak[cardnum-1] = 0;
    }

    else
    {
      Serial.println(": Nincs kartya.");
    }
  }

  //Write to serial the state of palcak[]
  Serial. print("Palcak: ");
  for(int i=0; i<8; i++)
  {
    Serial.print(palcak[i]);
    Serial.print(", ");
  }
  Serial.println("");


  byte data = 0;
  //Make 8-bit data from palcak[]
  for(int i=0; i<8; i++)
  {
    bitWrite(data, 7-i, palcak[i]);
  }
  sendData(data); //Send data through I2C

  for(int i=0; i<8; i++) palcak[i] = 1; //Reset palcak[]

  delay(500);
}



void sendData(byte data)
{
  // Inform the bus that we will be sending data to slave device 0x08
  Wire.beginTransmission(0x08);
  Wire.write(data); // Send the value
  Wire.endTransmission(); // Inform the bus and the slave device that we have finished sending data

  Serial.print("Sent data in HEX: ");
  Serial.println(data, HEX); // Print the hexadecimal representation of the sent data
}



void dump_byte_array(byte *buffer, byte bufferSize, byte &cardnum)
{
  char cardid[NR_OF_READERS];
  String cardid1 = "";
  cardnum = 0;

  for(int i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    sprintf(cardid, "%d", buffer[i]);
    cardid1 += (cardid);
  }
  Serial.print(cardid1);

  for(int i = 0; i < NR_OF_READERS; i++)
  {
    if(card_ids[i] == cardid1)
    {
      Serial.print(" Jo kartya, sorszama: ");
      Serial.println(i+1);
      cardnum = i+1;
    }
  }
  if(!cardnum) Serial.println(" Nem jo kartya.");
}