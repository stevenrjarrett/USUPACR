// Simple program to verify that stuff is working. This will send how much time has elapsed every .5 seconds.

#include <Servo.h>
#define DELAYTIME 200
#define LMOTOR 5
#define RMOTOR 6

int lMotor = 0;
int rMotor = 0;
unsigned long lastTime = 0;

void setup()
{
  Serial.begin(9600);
  analogWrite(LMOTOR, 0);
  analogWrite(RMOTOR, 0);
  
}


void loop()
{
  
  // Input value from console
  int in_val = 0;
  while (Serial.available()>1)
  {
    lMotor = Serial.parseInt();
    rMotor = Serial.parseInt();
    analogWrite(LMOTOR, abs(lMotor));
    analogWrite(RMOTOR, abs(rMotor));
    lastTime = millis();
  }
//  if(Serial.available() == 1)
//    Serial.parseInt();

  // Safety: If I haven't heard anything from Serial for a while, 
  if(lastTime - millis() > 5000)
  {
    analogWrite(LMOTOR, 0);
    analogWrite(RMOTOR, 0);
  }
  
  delay(DELAYTIME);
  
  
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
