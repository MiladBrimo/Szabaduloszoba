#define led1 6
#define led2 5
#define led3 4
#define led4 3
#define led5 2

#define pot A0
#define uv 10

byte state = 5;
byte last_state = 6;

long last_millis = 0;
byte done = false;
int kesleltetes = 1000;


void setup()
{
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
pinMode(led4, OUTPUT);
pinMode(led5, OUTPUT);
pinMode(uv, OUTPUT);
pinMode(pot, INPUT);
}


void loop()
{
  state = map(analogRead(pot), 0, 1023, 23, 0);

  if(state != last_state)
  {
    last_millis = millis();
    last_state = state;
    done = false;
  }

  if(millis() - last_millis > kesleltetes && !done)
  {
    switch(state)
    {
      case 21 ... 23:
      digitalWrite(led5, HIGH);
      digitalWrite(led4, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led1, HIGH);
      digitalWrite(uv, HIGH);
      break;

      case 18 ... 20:
      digitalWrite(led5, LOW);
      digitalWrite(led4, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led1, HIGH);
      digitalWrite(uv, HIGH);
      break;

      case 16 ... 17:
      digitalWrite(led5, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led3, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led1, HIGH);
      digitalWrite(uv, HIGH);
      break;

      case 13 ... 15:
      digitalWrite(led5, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led1, HIGH);
      digitalWrite(uv, HIGH);
      break;

      case 11 ... 12:
      digitalWrite(led5, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led1, HIGH);
      digitalWrite(uv, HIGH);
      break;

      case 8 ... 10:
      digitalWrite(led5, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led1, LOW);
      digitalWrite(uv, LOW);
      break;

      case 0 ... 7:
      digitalWrite(led5, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(led3, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led1, LOW);
      digitalWrite(uv, HIGH);
      break;
    }

    done = true;
  }

  delay(50);
}