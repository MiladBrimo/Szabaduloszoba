#include <SPI.h>
#include <MFRC522.h>


#define lockpin 9

#define RST_PIN 2
#define SS_1_PIN 3
#define SS_2_PIN 4
#define SS_3_PIN 5
#define SS_4_PIN 6 
#define SS_5_PIN 7

#define NR_OF_READERS   5

#define state_Start 0
#define state_1 1
#define state_2 2
#define state_3 3
#define state_4 4
#define state_5 5
#define state_6 6
#define state_End 7

#define minta_1 1 //fent
#define minta_2 2 //bal
#define minta_3 3 //alul
#define minta_4 4 //jobb
#define minta_5 1 //fent
#define minta_6 3 //alul
#define minta_7 5 //legalul


const byte ssPins[] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN, SS_5_PIN};
MFRC522 mfrc522[NR_OF_READERS];

byte state = state_Start;

unsigned long millis_val = 0;
const int millis_delay = 7000;



void setup()
{
	Serial.begin(9600); // Setup Serial- Monitor
  SPI.begin(); // Init SPI bus

  pinMode(lockpin, OUTPUT);
  digitalWrite(lockpin, LOW);

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
  if(millis() - millis_val > millis_delay) state = state_Start;
  Serial.println(""); Serial.print("State: "); Serial.println(state);

  byte RFID_num = readRFIDs();

  switch (state)
  {

    case state_Start:
      if(RFID_num == minta_1)
      {
        state = state_1;
        millis_val = millis();
      }

      else
      {
        if(RFID_num)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_1:
      if(RFID_num == minta_2)
      {
        state = state_2;
        millis_val = millis();
      }

      else
      {
        if(RFID_num && RFID_num != minta_1)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_2:
      if(RFID_num == minta_3)
      {
        state = state_3;
        millis_val = millis();
      }

      else
      {
        if(RFID_num && RFID_num != minta_2)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_3:
      if(RFID_num == minta_4)
      {
        state = state_4;
        millis_val = millis();
      }

      else
      {
        if(RFID_num && RFID_num != minta_3)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_4:
      if(RFID_num == minta_5)
      {
        state = state_5;
        millis_val = millis();
      }

      else
      {
        if(RFID_num && RFID_num != minta_4)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_5:
      if(RFID_num == minta_6)
      {
        state = state_6;
        millis_val = millis();
      }

      else
      {
        if(RFID_num && RFID_num != minta_5)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_6:
      if(RFID_num == minta_7)
      {
        state = state_End;
      }

      else
      {
        if(RFID_num && RFID_num != minta_6)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_End:
      delay(500);
      Serial.println("Nyitas!");
      digitalWrite(lockpin, HIGH);
      delay(3000);
      digitalWrite(lockpin, LOW);
      state = state_Start;
    break;

  }

  delay(10);
}



byte readRFIDs()
{
  byte reader_num = 0;
  
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    // Look for new cards
    //mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    //Serial.print("Reader "); Serial.print(reader+1);

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial())
    {
      mfrc522[reader].PICC_HaltA(); // Halt PICC
      mfrc522[reader].PCD_StopCrypto1(); // Stop encryption on PCD
      //Serial.println(": Van kartya.");
      reader_num = reader+1;
    }

    else
    {
      //Serial.println(": Nincs kartya.");
    }
  }

  Serial.print("RFID olvaso: "); Serial.println(reader_num);
  return reader_num;
}