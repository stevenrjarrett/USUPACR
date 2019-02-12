// Simple program to verify that stuff is working. This will send how much time has elapsed every .5 seconds.

#include <Servo.h>
#define DELAYTIME 100

Servo lMotor;

void setup()
{
  Serial.begin(9600);
  lMotor.attach(8);
  lMotor.writeMicroseconds(1000);
  
}


void loop()
{
  
  // Input value from console
  int in_val = 0;
  if (Serial.available()) {
    int in_val = Serial.parseInt();
    int in_servoVal = map(in_val, 0, 1023, 1000, 2000);
    int in_pwmVal  = map(in_val, 0, 1023, 0, 255);
    lMotor.writeMicroseconds(in_val);
    analogWrite(5, in_pwmVal);
    Serial.print("in_val = ");
    Serial.println(in_val);
    Serial.print("\n");
    delay(DELAYTIME);
  }
  
  
  /* // Read in value from potentiometer
  int val = analogRead(A0);
  int servoVal = map(val, 0, 1023, 1000, 2000);
  int pwmVal  = map(val, 0, 1023, 0, 255);
  lMotor.writeMicroseconds(val);
  analogWrite(5, pwmVal);
  
  Serial.print("pwmVal = ");
  Serial.print(pwmVal);
  Serial.print("\n");
  delay(DELAYTIME);
  */
}
