#include "AS5600.h"

#include <SPI.h>
#include <MFRC522.h>

#include <Adafruit_NeoPixel.h>


#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define LED_PIN 6
#define NUM_LEDS 24

#define DO_PIN 3
#define DI_PIN 2


AS5600L as5600; //use default Wire
byte angle = 0; //0...NUM_LEDS
byte True_Angle = 12;

byte ScannedID[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const byte AccessCard[7] = {0x04, 0x45, 0xE5, 0x04, 0xBC, 0x2A, 0x81};
bool match = false;
byte fail_counter = 0;

Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);



void setup()
{
  Serial.begin(9600);
  Wire.begin();
  as5600.begin(255); //set direction pin - ha nem akarjuk, akkor (255) és DIR-->GND
  as5600.setDirection(AS5600_CLOCK_WISE); //default, just be explicit.
  
  Serial.print("Connect: ");
  Serial.println(as5600.isConnected());
  delay(500);

  SPI.begin();
  mfrc522.PCD_Init();

  pixels.begin();

  pinMode(DI_PIN, INPUT);
  pinMode(DO_PIN, OUTPUT); digitalWrite(DO_PIN, LOW);
}



void loop()
{
  
  if(True_RFID())
  {

    angle = map(as5600.readAngle(), 0, 4095, 0, NUM_LEDS-1); //0...NUM_LEDS
    Serial.print("Angle: ");
    Serial.println(angle);

    for(int i = 0; i < NUM_LEDS; i++) pixels.setPixelColor(i, pixels.Color(0,255,0) ); //Green
    pixels.setPixelColor(angle, pixels.Color(0,0,255) ); //Blue
    pixels.show(); //Update LED Ring

    Serial.print("DI_PIN status: ");
    Serial.println(digitalRead(DI_PIN));
    
    if(angle == True_Angle && digitalRead(DI_PIN) )
    {
      digitalWrite(DO_PIN, HIGH);
      Serial.println("DO_PIN HIGH!");
    }
    else
    {
      digitalWrite(DO_PIN, LOW);
      Serial.println("DO_PIN LOW!");
    }

  }

  else
  {
    for(int i = 0; i < NUM_LEDS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(255,0,0) ); //Red
    }
    pixels.show(); //Update LED Ring

    digitalWrite(DO_PIN, LOW);
  }

  Serial.println("");
  delay(50);
}



bool True_RFID()
{

  mfrc522.PCD_Init();
  delay(10);

  if(mfrc522.PICC_IsNewCardPresent())
  {
    if(mfrc522.PICC_ReadCardSerial())
    {

      fail_counter = 0;


      //getID();
      for (int i = 0; i < 7; i++)
      {
        ScannedID[i] = mfrc522.uid.uidByte[i];
      }
      for (int i = 0; i < 7; i++)
      {
        Serial.print(ScannedID[i], HEX);
        Serial.print(" ");
      }
      Serial.println();


      //checkCard();
      match = true;
      for(int i = 0; i < 7; i++)
      {
        if(ScannedID[i] != AccessCard[i]) match = false;
      }


      //isMatch();
      if(match)
      {
        Serial.println(F("Jo Serleg!"));
        return true;
      }
      else
      {
        Serial.println(F("Rossz Serleg!"));
        return false;
      }

    }
  }

  else
  {
    fail_counter++;

    if(fail_counter == 2)
    {
      fail_counter = 0;

      Serial.println("Nincs Serleg.");
      return false;
    }
  }

}