#include <FourBitLedDigitalTube.h>

const uint8_t dio {2};
const uint8_t rclk {3};
const uint8_t sclk {4};

bool testResult{};

TM74HC595LedTube myLedDisp(sclk, rclk, dio);


#define R1 6
#define R2 11
#define R3 10
#define R4 8
#define C1 7
#define C2 5
#define C3 9
#define lock 13


int PINCODE = 924;
int INPUTCODE = 0;

byte digit = 0;
byte num = 69;
bool nullFirst = false;



void setup()
{

 pinMode(R1, INPUT_PULLUP);
 pinMode(R2, INPUT_PULLUP);
 pinMode(R3, INPUT_PULLUP);
 pinMode(R4, INPUT_PULLUP);
 pinMode(C1, OUTPUT);
 pinMode(C2, OUTPUT);
 pinMode(C3, OUTPUT);
 pinMode(lock, OUTPUT);

 digitalWrite(C1, HIGH);
 digitalWrite(C2, HIGH);
 digitalWrite(C3, HIGH);
 digitalWrite(lock, HIGH);

 testResult = myLedDisp.print("----");

}



void loop()
{

 myLedDisp.begin();
 num = keyScan();


 if(num < 10 && digit < 4)
 {
   INPUTCODE = 10*INPUTCODE + num;
   testResult = myLedDisp.print(INPUTCODE, true, false);
   
   if(digit == 0 && num == 0) nullFirst = true;

   if(nullFirst) testResult = myLedDisp.write("0", digit);

   digit++;
   delay(250);
 }
  

 if(num == 11)
 {
   INPUTCODE = 0;
   digit = 0;
   nullFirst = false;
   testResult = myLedDisp.print("----");
 }
 

 if(num == 12)
 {
   if(INPUTCODE == PINCODE)
   {
     testResult = myLedDisp.print("OPEn");
     digitalWrite(lock, LOW);
     delay(3000);

     digitalWrite(lock, HIGH);
     INPUTCODE = 0;
     digit = 0;
     nullFirst = false;
     testResult = myLedDisp.print("----");
   }
   else
   {
     testResult = myLedDisp.print("FALS");
     delay(3000);

     INPUTCODE = 0;
     digit = 0;
     nullFirst = false;
     testResult = myLedDisp.print("----");
   }
 }

}



byte keyScan()
{

 int key = 69;


 digitalWrite(C1, LOW);
 delay(1);
 if(!digitalRead(R1)) key = 1;
 if(!digitalRead(R2)) key = 4;
 if(!digitalRead(R3)) key = 7;
 if(!digitalRead(R4)) key = 11;
 delay(1);
 digitalWrite(C1, HIGH);

 digitalWrite(C2, LOW);
 delay(1);
 if(!digitalRead(R1)) key = 2;
 if(!digitalRead(R2)) key = 5;
 if(!digitalRead(R3)) key = 8;
 if(!digitalRead(R4)) key = 0;
 delay(1);
 digitalWrite(C2, HIGH);

 digitalWrite(C3, LOW);
 delay(1);
 if(!digitalRead(R1)) key = 3;
 if(!digitalRead(R2)) key = 6;
 if(!digitalRead(R3)) key = 9;
 if(!digitalRead(R4)) key = 12;
 delay(1);
 digitalWrite(C3, HIGH);


 delay(20);
 return key;

}