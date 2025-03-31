#define MASTER_EN 2   // connected to RS485 Enable pin

const byte Red_Led[4] = {A1, A3, A5, 3};
const byte Green_Led[4] = {A0, A2, A4, 4};

const byte A_Socket[4] = {5, 7, 9, 11};
const byte B_Socket[4] = {6, 8, 10, 12};

byte State = 0;



void setup()
{

  pinMode(MASTER_EN , OUTPUT);      // Declare Enable pin as output
  Serial.begin(9600);               // set serial communication baudrate 
  digitalWrite(MASTER_EN , LOW);    // Make Enable pin low - Receiving mode ON

  for(int i = 0; i < 4; i++)
  {
    pinMode(Red_Led[i], OUTPUT);  digitalWrite(Red_Led[i], HIGH);
    pinMode(Green_Led[i], OUTPUT);  digitalWrite(Green_Led[i], LOW);
    pinMode(A_Socket[i], OUTPUT); digitalWrite(A_Socket[i], HIGH);
    pinMode(B_Socket[i], INPUT_PULLUP);
  }

}



void loop()
{

  Socket_Scan();

  digitalWrite(MASTER_EN , HIGH);     // Make Enable pin high to send Data
  delay(5);                           // required minimum delay of 5ms
  Serial.println(State);              // Send character A serially
  Serial.flush();                     // wait for transmission of data
  digitalWrite(MASTER_EN , LOW);      // Receiving mode ON

  while(State == 4)
  {
    for(int i = 0; i < 4; i++)
    {
      digitalWrite(Green_Led[i], HIGH);
      delay(1);
      digitalWrite(Green_Led[i], LOW);
    }
    
    digitalWrite(MASTER_EN , HIGH);     // Make Enable pin high to send Data
    delay(5);                           // required minimum delay of 5ms
    Serial.println(State);              // Send character A serially
    Serial.flush();                     // wait for transmission of data
    digitalWrite(MASTER_EN , LOW);     // Receiving mode ON
  }

}



void Socket_Scan()
{

  State = 0;

  for(int i = 0; i < 4; i++)
  {
    digitalWrite(A_Socket[i], LOW);
    delay(1);

    for(int j = 0; j < 4; j++)
    {
      digitalWrite(Green_Led[j], LOW);
      digitalWrite(Red_Led[j], LOW);
    }

    if(!digitalRead(B_Socket[i]))
    {
      State++;
      digitalWrite(Green_Led[i], HIGH);
    }
    else
    {
      digitalWrite(Red_Led[i], HIGH);
    }

    delay(1);
    digitalWrite(A_Socket[i], HIGH);
  }

}