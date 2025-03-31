#define REED_1_PIN 3
#define REED_2_PIN 4
#define REED_3_PIN 5
#define REED_4_PIN 6 
const byte REED[4] = {REED_1_PIN, REED_2_PIN, REED_3_PIN, REED_4_PIN};

#define RELAY_1_PIN 7
#define RELAY_2_PIN 8
#define RELAY_3_PIN 9
#define RELAY_4_PIN 10 
const byte RELAY[4] = {RELAY_1_PIN, RELAY_2_PIN, RELAY_3_PIN, RELAY_4_PIN};

#define NR_OF_READERS 4



void setup()
{
  
  for (int i = 0; i < NR_OF_READERS; i++)
  {
    pinMode(REED[i], INPUT_PULLUP);
    pinMode(RELAY[i], OUTPUT);
    digitalWrite(RELAY[i], LOW);
  }

}



void loop()
{
  if( !digitalRead(REED[0]) && !digitalRead(REED[1]) && !digitalRead(REED[2]) && !digitalRead(REED[3]) )
  {
    digitalWrite(RELAY[0], HIGH);
    delay(1000);

    digitalWrite(RELAY[1], HIGH);
    delay(1000);

    digitalWrite(RELAY[2], HIGH);
    delay(1000);

    digitalWrite(RELAY[3], HIGH);
    delay(5000);

    digitalWrite(RELAY[0], LOW);
    digitalWrite(RELAY[1], LOW);
    digitalWrite(RELAY[2], LOW);
    digitalWrite(RELAY[3], LOW);
  }

  delay(10);
}