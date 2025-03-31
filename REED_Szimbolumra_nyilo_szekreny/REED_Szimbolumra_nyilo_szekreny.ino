#define lockpin 9

#define REED_1_PIN 3
#define REED_2_PIN 4
#define REED_3_PIN 5
#define REED_4_PIN 6 
#define REED_5_PIN 7
const byte REED[5] = {REED_1_PIN, REED_2_PIN, REED_3_PIN, REED_4_PIN, REED_5_PIN};

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


byte state = state_Start;

unsigned long millis_val = 0;
const int millis_delay = 7000;



void setup()
{
	Serial.begin(9600); // Setup Serial- Monitor

  pinMode(lockpin, OUTPUT);
  digitalWrite(lockpin, LOW);

  pinMode(REED_1_PIN, INPUT_PULLUP);
  pinMode(REED_2_PIN, INPUT_PULLUP);
  pinMode(REED_3_PIN, INPUT_PULLUP);
  pinMode(REED_4_PIN, INPUT_PULLUP);
  pinMode(REED_5_PIN, INPUT_PULLUP);
}



void loop()
{
  if(millis() - millis_val > millis_delay) state = state_Start;
  Serial.println(""); Serial.print("State: "); Serial.println(state);

  byte REED_num = readREEDs();

  switch (state)
  {

    case state_Start:
      if(REED_num == minta_1)
      {
        state = state_1;
        millis_val = millis();
      }

      else
      {
        if(REED_num)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_1:
      if(REED_num == minta_2)
      {
        state = state_2;
        millis_val = millis();
      }

      else
      {
        if(REED_num && REED_num != minta_1)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_2:
      if(REED_num == minta_3)
      {
        state = state_3;
        millis_val = millis();
      }

      else
      {
        if(REED_num && REED_num != minta_2)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_3:
      if(REED_num == minta_4)
      {
        state = state_4;
        millis_val = millis();
      }

      else
      {
        if(REED_num && REED_num != minta_3)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_4:
      if(REED_num == minta_5)
      {
        state = state_5;
        millis_val = millis();
      }

      else
      {
        if(REED_num && REED_num != minta_4)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_5:
      if(REED_num == minta_6)
      {
        state = state_6;
        millis_val = millis();
      }

      else
      {
        if(REED_num && REED_num != minta_5)
        {
          Serial.println("Hibas minta!");
          state = state_Start;
        }
      }
    break;


    case state_6:
      if(REED_num == minta_7)
      {
        state = state_End;
      }

      else
      {
        if(REED_num && REED_num != minta_6)
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



byte readREEDs()
{
  byte reader_num = 0;
  
  for (int reader = 0; reader < NR_OF_READERS; reader++)
  {
    Serial.print("Reader "); Serial.print(reader+1);

    if (!digitalRead(REED[reader]))
    {
      Serial.println(": Van palca.");
      reader_num = reader+1;
    }

    else
    {
      Serial.println(": Nincs palca.");
    }
  }

  Serial.print("REED olvaso: "); Serial.println(reader_num);
  return reader_num;
}