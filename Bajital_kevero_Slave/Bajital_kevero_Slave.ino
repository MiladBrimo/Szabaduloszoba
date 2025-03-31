#include <Wire.h>

#include <SPI.h>
#include <MFRC522.h>


#define RST_PIN 2
#define SS_1_PIN 7
#define SS_2_PIN 6
#define SS_3_PIN 5
#define SS_4_PIN 4
#define SS_5_PIN 3

#define NR_OF_READERS 5
#define NR_OF_CARDS 10

const byte ssPins[NR_OF_READERS] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN, SS_5_PIN};
MFRC522 mfrc522[NR_OF_READERS];

const String card_ids[NR_OF_CARDS] = {"1846111818", "1682315418", "16814210218", "12812719085", "18415821518", "184916718", "184786518", "18418018518", "16813114118", "1681389218"};
byte readers_state[NR_OF_READERS] = {0, 0, 0, 0, 0};



void setup()
{
	Serial.begin(9600); // Setup Serial Monitor
  Wire.begin(8); // join I2C bus with address 8
  SPI.begin(); // Init SPI bus

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print("Reader ");
    Serial.print(reader+6);
    Serial.print(": ");
    mfrc522[reader].PCD_DumpVersionToSerial();
    delay(50);
  }

  Wire.onRequest(requestEvent); // register request event
}



void loop()
{
  Serial.println("");

  // Look for new cards
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print("Reader ");
    Serial.print(reader+6);

    if (mfrc522[reader].PICC_IsNewCardPresent()&& mfrc522[reader].PICC_ReadCardSerial())
    {
      Serial.print(": Card UID:");
      byte cardnum = 0;
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size, cardnum);

      mfrc522[reader].PICC_HaltA(); // Halt PICC
      mfrc522[reader].PCD_StopCrypto1(); // Stop encryption on PCD

      readers_state[reader] = cardnum;
    }

    else
    {
      Serial.println(": Nincs fiola.");
      readers_state[reader] = 0;
    }
  }


  //Write to serial the state of readers_state[]
  Serial.print("Readers_state: ");
  for(int i=0; i<NR_OF_READERS; i++)
  {
    Serial.print(readers_state[i]);
    Serial.print(", ");
  }
  Serial.println("");


  delay(10);
}



void dump_byte_array(byte *buffer, byte bufferSize, byte &cardnum)
{
  String cardid = "";

  for(int i = 0; i < bufferSize; i++)
  {
    cardid += String(buffer[i], DEC); //Hexa kiiratashoz: String(buffer[i], HEX);
  }
  Serial.println(cardid);

  for(int i = 0; i < NR_OF_CARDS; i++)
  {
    if(card_ids[i] == cardid) cardnum = i+1;
  }
}



void requestEvent()
{
 for(int i = 0; i < NR_OF_READERS; i++)
 {
   Wire.write(readers_state[i]);
 }
}