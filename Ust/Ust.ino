#define EN 4
#define CLK 5 
#define DIR 6

#define INTRPT 2
#define FAN1 9
#define FAN2 10

#define STEP_POT A0
#define FAN1_POT A1
#define FAN2_POT A2


volatile bool flag = false;
byte Fan1_Speed = 100;
byte Fan2_Speed = 100;
int Step_Speed = 5000;


void setup()
{  
  pinMode(EN, OUTPUT); digitalWrite(EN, HIGH);
  pinMode(CLK, OUTPUT);
  pinMode(DIR, OUTPUT); digitalWrite(DIR, HIGH);

  pinMode(INTRPT, INPUT);
  pinMode(FAN1, OUTPUT);
  pinMode(FAN2, OUTPUT);

  pinMode(STEP_POT, INPUT);
  pinMode(FAN1_POT, INPUT);
  pinMode(FAN2_POT, INPUT);

  Step_Speed = map(analogRead(STEP_POT), 0, 1023, 12500, 1250);
  Fan1_Speed = map(analogRead(FAN1_POT), 0, 1023, 0, 255); analogWrite(FAN1, Fan1_Speed);
  Fan2_Speed = map(analogRead(FAN2_POT), 0, 1023, 0, 255); analogWrite(FAN2, Fan2_Speed);

  attachInterrupt(digitalPinToInterrupt(INTRPT), Read_Pot, RISING);
}



void loop()
{

  if(flag)
  {
    flag = false;

    Step_Speed = map(analogRead(STEP_POT), 0, 1023, 12500, 1250);

    Fan1_Speed = map(analogRead(FAN1_POT), 0, 1023, 0, 255);
    analogWrite(FAN1, Fan1_Speed);
    
    Fan2_Speed = map(analogRead(FAN2_POT), 0, 1023, 0, 255);
    analogWrite(FAN2, Fan2_Speed);
  }


  for(int i=0; i<400; i++)
  {
    digitalWrite(CLK, HIGH);
    delayMicroseconds(Step_Speed);
    digitalWrite(CLK, LOW);
    delayMicroseconds(Step_Speed);
  }

}



void Read_Pot()
{
  flag = true;
}