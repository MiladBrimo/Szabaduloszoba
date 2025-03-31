#define REED_1_PIN 7
#define REED_2_PIN 6
#define REED_3_PIN 5
#define REED_4_PIN 4 
#define REED_5_PIN 3
const byte REED[5] = {REED_1_PIN, REED_2_PIN, REED_3_PIN, REED_4_PIN, REED_5_PIN};

#define LOCK_PIN 9
#define BUZZ_PIN 8


#define NR_OF_READERS 3

bool readers_state[NR_OF_READERS] = {false, false, false};
bool true_comb[NR_OF_READERS] = {true, true, true};

bool solved = false;
bool waschange = true;



void setup()
{
	Serial.begin(9600); // Setup Serial Monitor

  for(int i = 0; i < NR_OF_READERS; i++) pinMode(REED[i], INPUT_PULLUP);

  pinMode(LOCK_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(LOCK_PIN, HIGH);
  digitalWrite(BUZZ_PIN, LOW);
}



void loop()
{
  Serial.println("");

  // Look for new cards
  for (int reader = 0; reader < NR_OF_READERS; reader++)
  {

    if(!digitalRead(REED[reader]))
    {
      Serial.print(": Van targy, helye: ");
      Serial.println(reader + 1);
      readers_state[reader] = true;
    }
    else
    {
      Serial.println(": Nincs targy.");
      readers_state[reader] = false;
    }
  }


  solved = true;
  for(int i = 0; i < NR_OF_READERS; i++) if(readers_state[i] != true_comb[i]) solved = false;

  if(!waschange)
  {
    if(!readers_state[0] && !readers_state[1] && !readers_state[2]) waschange = true;
  }

  if(solved && waschange)
  {
    digitalWrite(LOCK_PIN, LOW);
    digitalWrite(BUZZ_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZ_PIN, LOW);
    delay(4500);
    digitalWrite(LOCK_PIN, HIGH);
    waschange = false;
  }


  delay(100);
}