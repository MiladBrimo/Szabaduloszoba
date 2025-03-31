#define Key_1 2
#define Key_2 3
#define Relay 4

unsigned long timer_millis = 0;
int timer_delay = 500;
int open_time = 10000;



void setup()
{
  pinMode(Key_1, INPUT_PULLUP);
  pinMode(Key_2, INPUT_PULLUP);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);
}



void loop()
{

  if(!digitalRead(Key_1) || !digitalRead(Key_2))
  {
    timer_millis = millis();

    while(millis() - timer_millis <= timer_delay)
    {
      if(!digitalRead(Key_1) && !digitalRead(Key_2))
      {
        digitalWrite(Relay, HIGH);
        delay(open_time);
        digitalWrite(Relay, LOW);
      }

      delay(1);
    }

    while(!digitalRead(Key_1) || !digitalRead(Key_2)) delay(10);
  }

}