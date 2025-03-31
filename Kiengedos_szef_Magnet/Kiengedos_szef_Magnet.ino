#define buzzer 2
#define red 3
#define green 4
#define lock 12

const long titlas_ido = 60000;
const byte hiba_hatar = 3;
byte hiba_szamlalo = 0;

const byte ROW[4] = {6, 11, 10, 8};
const byte COL[3] = {7, 5, 9};


int PINCODE = 1234;
int INPUTCODE = 0;

byte num = 69;



void setup()
{

  for(int i = 0; i < 4; i++) pinMode(ROW[i], INPUT_PULLUP);
  for(int i = 0; i < 3; i++) pinMode(COL[i], OUTPUT);
  for(int i = 0; i < 3; i++) digitalWrite(COL[i], HIGH);

  pinMode(buzzer, OUTPUT); digitalWrite(buzzer, LOW);
  pinMode(red, OUTPUT); digitalWrite(red, LOW);
  pinMode(green, OUTPUT); digitalWrite(green, LOW);
  pinMode(lock, OUTPUT); digitalWrite(lock, HIGH);

}



void loop()
{

  num = keyScan();


  if(num < 10) INPUTCODE = 10*INPUTCODE + num;
  

  if(num == 10) INPUTCODE = 0;
 

  if(hiba_szamlalo >= hiba_hatar)
  {
    delay(titlas_ido);
    hiba_szamlalo = 0;
  }


  if(num == 12)
  {
    if(INPUTCODE == PINCODE)
    {
      digitalWrite(lock, LOW);
      digitalWrite(buzzer, HIGH);
      digitalWrite(green, HIGH);
      delay(3000);
      digitalWrite(lock, HIGH);
      digitalWrite(buzzer, LOW);
      digitalWrite(green, LOW);

      INPUTCODE = 0;
      hiba_szamlalo = 0;
    }
    else
    {
      for(int i = 0; i < 3; i++)
      {
        digitalWrite(red, HIGH);
        digitalWrite(buzzer, HIGH);
        delay(100);
        digitalWrite(red, LOW);
        digitalWrite(buzzer, LOW);
        delay(250);
      }

      INPUTCODE = 0;
      hiba_szamlalo++;
    }
  }

}



byte keyScan()
{

  byte key = 69;

  for(int i = 0; i < 3; i++)
  {
    digitalWrite(COL[i], LOW);
    delay(1);

    for(int j = 0; j < 4; j++)
    {
      if(!digitalRead(ROW[j]))
      {
        key = 3*j + i + 1;
        while(!digitalRead(ROW[j])) delay(1);
        digitalWrite(buzzer, HIGH); //Csipanas
        delay(30);
        digitalWrite(buzzer, LOW);
      }
      else delay(8);
    }

    delay(1);
    digitalWrite(COL[i], HIGH);
  }

  if(key == 11) key = 0; //0-as gomb

  return key;

}