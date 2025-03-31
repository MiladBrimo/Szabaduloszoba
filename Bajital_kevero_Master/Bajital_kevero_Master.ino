#include <Wire.h>

#include <SPI.h>
#include <MFRC522.h>

#include <Adafruit_NeoPixel.h>


#define RST_PIN 2
#define SS_1_PIN 3
#define SS_2_PIN 4
#define SS_3_PIN 5
#define SS_4_PIN 6
#define SS_5_PIN 7

#define NR_OF_READERS 5
#define NR_OF_CARDS 10

#define LED_PIN 8
#define NUM_LEDS 20

#define LOCK_PIN 9


const byte ssPins[NR_OF_READERS] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN, SS_5_PIN};
MFRC522 mfrc522[NR_OF_READERS];

const String card_ids[NR_OF_CARDS] = {"1846111818", "1682315418", "16814210218", "12812719085", "18415821518", "184916718", "184786518", "18418018518", "16813114118", "1681389218"};
byte readers_state[NR_OF_CARDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte last_readers_state[NR_OF_CARDS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte pattern_readers_state[NR_OF_CARDS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
byte red=255, green=255, blue=0;
byte last_red=255, last_green=255, last_blue=0;
byte old_red=255, old_green=255, old_blue=0;
int led_step = 0;
const int step_time = 5;

bool opened = false;
const int open_delay = 3000;
const int colorChange_delay = 3000;
int colorChange_counter = 0;
unsigned long last_millis = 0;



void setup()
{
  Serial.begin(9600);
  Wire.begin();
  SPI.begin();
  pixels.begin();

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print("Reader ");
    Serial.print(reader+1);
    Serial.print(": ");
    mfrc522[reader].PCD_DumpVersionToSerial();
    delay(50);
  }

  pinMode(LOCK_PIN, OUTPUT);
  digitalWrite(LOCK_PIN, LOW);

}



void loop()
{
  Serial.println("");
  bool all_correct = true; //Flag to track if all readers are correct
  byte state_change = 0; //Flag when the state of readers change


  // Look for new cards
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    circleAnimation(red, green, blue);
    
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    //Serial.print("Reader ");
    //Serial.print(reader+1);

    if (mfrc522[reader].PICC_IsNewCardPresent()&& mfrc522[reader].PICC_ReadCardSerial())
    {
      //Serial.print(": Card UID:");
      byte cardnum = 0;
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size, cardnum);

      mfrc522[reader].PICC_HaltA(); // Halt PICC
      mfrc522[reader].PCD_StopCrypto1(); // Stop encryption on PCD

      readers_state[reader] = cardnum;
    }
    else
    {
      //Serial.println(": Nincs fiola.");
      readers_state[reader] = 0;
    }
  }


  //Request data from slave
  Wire.requestFrom(8, NR_OF_READERS);
  while (Wire.available() < NR_OF_READERS) delay(1); //Wait for response
  for (int i = NR_OF_READERS; i < NR_OF_CARDS; i++) readers_state[i] = Wire.read(); //Read data into second half of readers_state array


  //Write to serial the state of readers_state[]
  Serial.print("Readers_state: ");
  for(int i = 0; i < NR_OF_CARDS; i++)
  {
    Serial.print(readers_state[i]);
    Serial.print(", ");
  }
  Serial.println("");


  //Manage the RFID state chnages
  for(int i = 0; i < NR_OF_CARDS; i++)
  {
    if(readers_state[i] != last_readers_state[i] && readers_state[i])
    {
      state_change = i+1;
      opened = false;
    }
  }


  //Check if "all correct"
  for(int i = 0; i < NR_OF_CARDS; i++)
  {
    if(readers_state[i] != pattern_readers_state[i]) all_correct = false;
  }


  //Set LED colors based on the reader states
  if (all_correct)
  {
    red=0; green=255; blue=0; //Green

    if(!opened)
    {
      digitalWrite(LOCK_PIN, HIGH);
      opened = true;
      last_millis = millis();
    }
    if(millis() - last_millis > open_delay) digitalWrite(LOCK_PIN, LOW);
  }

  else
  {

    if(state_change)
    {
      if(readers_state[state_change-1] == pattern_readers_state[state_change-1])
      {
        red=0; green=0; blue=255; //Blue
        last_millis = millis();
      }
      else
      {
        red=255; green=0; blue=0; //Red
        last_millis = millis();
      }
    }
    else
    {
      if(millis() - last_millis > colorChange_delay)
      {
      red=255; green=255; blue=0; //Yellow
      }
    }

  }


  //Update last_readers_state
  for(int i = 0; i < NR_OF_CARDS; i++) last_readers_state[i] = readers_state[i];


  //delay(1);
}



void dump_byte_array(byte *buffer, byte bufferSize, byte &cardnum)
{
  String cardid = "";

  for(int i = 0; i < bufferSize; i++)
  {
    cardid += String(buffer[i], DEC); //Hexa kiiratashoz: String(buffer[i], HEX);
  }
  //Serial.println(cardid);

  for(int i = 0; i < NR_OF_CARDS; i++)
  {
    if(card_ids[i] == cardid) cardnum = i+1;
  }
}



void circleAnimation(byte r, byte g, byte b)
{
  if(last_red != r || last_green != g || last_blue != b)
  {
    colorChange_counter = 0;
    last_red = r; last_green = g; last_blue = b;
  }
  colorChange_counter++;

  if(colorChange_counter >= NUM_LEDS)
  {
      old_red = r; old_green = g; old_blue = b;
  }
  
  for(int i = 0; i < NUM_LEDS; i++)
  {
    if(i<colorChange_counter) pixels.setPixelColor( (led_step-i < 0) ? led_step-i+NUM_LEDS : led_step-i, pixels.Color( ((NUM_LEDS-i-1)*r)/(NUM_LEDS-1), ((NUM_LEDS-i-1)*g)/(NUM_LEDS-1), ((NUM_LEDS-i-1)*b)/(NUM_LEDS-1) ));
    else pixels.setPixelColor( (led_step-i < 0) ? led_step-i+NUM_LEDS : led_step-i, pixels.Color( ((NUM_LEDS-i-1)*old_red)/(NUM_LEDS-1), ((NUM_LEDS-i-1)*old_green)/(NUM_LEDS-1), ((NUM_LEDS-i-1)*old_blue)/(NUM_LEDS-1) ));
  }

  pixels.show(); //Update LED strip
  delay(step_time);
  led_step++;
  if(led_step >= NUM_LEDS) led_step = 0;
}