#include <SPI.h>
#include <MFRC522.h>


#define RST_PIN 2
#define SS_1_PIN 7
#define SS_2_PIN 6
#define SS_3_PIN 5
#define SS_4_PIN 4
#define SS_5_PIN 3

#define LOCK_PIN 9
#define BUZZ_PIN 8

#define NR_OF_READERS 3
#define NR_OF_CARDS 3

const byte ssPins[NR_OF_READERS] = {SS_1_PIN, SS_2_PIN, SS_3_PIN};
MFRC522 mfrc522[NR_OF_READERS];

const String card_ids[NR_OF_CARDS] = {"4148156418842129", "414884418842129", "44578418842129"}; //Szekesfehervar
//const String card_ids[NR_OF_CARDS] = {"4148156418842129", "4136167418842129", "4130173418842129"}; //Pecs
byte readers_state[NR_OF_READERS] = {0, 0, 0};
byte true_comb[NR_OF_READERS] = {1, 2, 3};
bool used_object[NR_OF_READERS] = {false, false, false};

bool solved = false;
bool waschange = true;



void setup()
{
	Serial.begin(9600); // Setup Serial Monitor
  SPI.begin(); // Init SPI bus

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print("Reader ");
    Serial.print(reader+1);
    Serial.print(": ");
    mfrc522[reader].PCD_DumpVersionToSerial();
    delay(100);
  }

  pinMode(LOCK_PIN, OUTPUT);    digitalWrite(LOCK_PIN, HIGH);
  pinMode(BUZZ_PIN, OUTPUT);    digitalWrite(BUZZ_PIN, LOW);
}



void loop()
{
  Serial.println("");

  // Look for new cards
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print("Reader ");
    Serial.print(reader+1);

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
      Serial.println(": Nincs targy.");
      readers_state[reader] = 0;
    }
  }


  for(int i = 0; i < NR_OF_READERS; i++) if(readers_state[i] == true_comb[i]) used_object[i] = true;

  solved = true;
  for(int i = 0; i < NR_OF_READERS; i++) if(!used_object[i]) solved = false;

  if(!waschange) solved = false;
  
  if(!waschange && !readers_state[0] && !readers_state[1] && !readers_state[2])
  {      
    for(int i = 0; i < NR_OF_READERS; i++) used_object[i] = false;
    waschange = true;
    digitalWrite(LOCK_PIN, HIGH);
    delay(500);
  }

  if(solved)
  {
    digitalWrite(LOCK_PIN, LOW);
    digitalWrite(BUZZ_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZ_PIN, LOW);
    delay(4500);

    waschange = false;
  }


  delay(100);
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