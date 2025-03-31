#include <Adafruit_NeoPixel.h>

#define Lock 9 
#define SLAVE_EN 2
#define LED_PIN 8

#define NUM_LEDS 76
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

byte State = 0;
byte last_State = 0;

int speed_delay = 50;



void setup()
{
  pinMode(SLAVE_EN , OUTPUT);                   // Declare Enable pin as output
  Serial.begin(9600);                           // set serial communication baudrate 
  digitalWrite(SLAVE_EN , LOW);                 // Make Enable pin low, Receiving mode ON 

  pixels.begin();

  pinMode(Lock, OUTPUT);
  digitalWrite(Lock, HIGH);


  for(int i = 0; i < NUM_LEDS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }

}



void loop()
{

  while(Serial.available())                     // If serial data is available then enter into while loop
  {
    byte tmp = Serial.read();
    if(tmp > 47) State = tmp - 48;
  }


  if(State != last_State && State>=0 && State<=4)
  {

    if(State-last_State > 0)
    {
      for(int i = ( (4-last_State) * NUM_LEDS) / 4; i > ( (4-State) * NUM_LEDS) / 4; i--)
      {
        pixels.setPixelColor(i-1, pixels.Color(255, 0, 0));
        pixels.show();
        delay(speed_delay);
      }
    }

    if(State-last_State < 0)
    {
      for(int i = ( (4-last_State) * NUM_LEDS) / 4; i < ( (4-State) * NUM_LEDS) / 4; i++)
      {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        pixels.show();
        delay(speed_delay);
      }
    }


    if(State == 4)
    {
      digitalWrite(Lock, LOW);
      delay(100);
    }
    else
    {
      digitalWrite(Lock, HIGH);
    }


    last_State = State;
  }

  delay(20);
}