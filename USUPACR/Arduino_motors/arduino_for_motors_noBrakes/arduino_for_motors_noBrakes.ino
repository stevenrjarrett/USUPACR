// Simple program to verify that stuff is working. This will send how much time has elapsed every .5 seconds.

#include <Servo.h>
#define DELAYTIME      200
#define ESTOP          2
#define LMOTOR         5
#define RMOTOR         6
#define LMOTOR_REVERSE 7
#define RMOTOR_REVERSE 8


int lMotor = 0;
int rMotor = 0;
unsigned long lastTime = 0;
bool Estop_engaged = false;

int sign(int val) { return val==0 ? 1 : val / abs(val);}

void setup()
{
  Serial.begin(9600);
  analogWrite(LMOTOR, 0);
  analogWrite(RMOTOR, 0);
  digitalWrite(LMOTOR_REVERSE,false);
  digitalWrite(RMOTOR_REVERSE,false);
}


void loop()
{
  bool shouldWrite = false;
  bool waitReverse = false;
  // Input value from console
//  int in_val = 0;
//  while (Serial.available()>2)
//  {
//    lMotor = Serial.parseInt();
//    rMotor = Serial.parseInt();
//  }
  while(Serial.available() > 1)
  {
    shouldWrite = true;
    // get motor input
    int lMotor_raw = Serial.parseInt();
    int rMotor_raw = Serial.parseInt();
    if(sign(lMotor_raw) == -sign(lMotor) || sign(rMotor_raw) == -sign(rMotor))
      waitReverse = true;
    if(abs(lMotor_raw) > 15)
      lMotor = map(abs(lMotor_raw), 0, 255, 40, 200);
    else
      lMotor = 0;
    if(abs(rMotor_raw) > 15)
      rMotor = map(abs(rMotor_raw), 0, 255, 40, 200);
    else
      rMotor = 0;
    if(lMotor_raw<0) lMotor = -lMotor;
    if(rMotor_raw<0) rMotor = -rMotor;
    
    lastTime = millis();
  }
//  if(Serial.available() == 1)
//    Serial.parseInt();

  // Safety: If I haven't heard anything from Serial for a while, 
  Estop_engaged = !digitalRead(ESTOP);
  if(lastTime - millis() > 500 || Estop_engaged)
  {
    analogWrite(LMOTOR, 0);
    analogWrite(RMOTOR, 0);
  }
  else if(shouldWrite)
  {
    digitalWrite(LMOTOR_REVERSE,lMotor<0);
    digitalWrite(RMOTOR_REVERSE,rMotor<0);
    if(waitReverse)
      delay(50);
    analogWrite(LMOTOR, abs(lMotor));
    analogWrite(RMOTOR, abs(rMotor));
    
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

