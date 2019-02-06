// Simple program to verify that stuff is working. This will send how much time has elapsed every .5 seconds.

#include <Servo.h>
#define DELAYTIME 500

Servo lMotor;

void setup()
{
  Serial.begin(9600);
  lMotor.attach(8);
  lMotor.writeMicroseconds(1000);
  
  
}


void loop()
{
  int val = analogRead(A0);
  int servoVal = map(val, 0, 1023, 1000, 2000);
  int pwmVal  = map(val, 0, 1023, 0, 255);
  lMotor.writeMicroseconds(val);
  analogWrite(3, pwmVal);
  
  Serial.print("pwmVal = ");
  Serial.print(pwmVal);
  Serial.print("\n");
  delay(DELAYTIME);
  
}
