#include <Servo.h>
Servo servo_pin_2;


void setup()
{

servo_pin_2.attach(2,530,2600);


servo_pin_2.write(60);
delay(1000);
servo_pin_2.write(130);

}


void loop()
{

}
