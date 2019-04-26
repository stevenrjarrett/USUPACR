// Simple program to verify that stuff is working. This will send how much time has elapsed every .5 seconds.

#include <Servo.h>
#include "AX12A.h"
#define DELAYTIME      100
#define ESTOP          2

#define DirectionPin   (10u)
#define BaudRate       (1000000ul)
#define ID             1
#define ID2            2

int lMotor     = 0;
int rMotor     = 0;
float lBrake   = 0;
//float rBrake   = 0;
unsigned long lastTime = 0;
bool Estop_engaged = false;

int brake_engaged = 400;
int brake_disengaged = 900;
int brake_pos = brake_disengaged;
bool shouldWrite = false;
bool waitReverse = false;

int sign(int val) { return val==0 ? 1 : val / abs(val);}

void setup()
{
  pinMode(ESTOP, INPUT);
//  Serial.begin(BaudRate);
  
  
  // Set up servos
  ax12a.begin(BaudRate, DirectionPin, &Serial);
  ax12a.reset(ID);
//  ax12a.reset(ID2);
//  ax12a.torqueStatus(ID, ON);
//  ax12a.setMaxTorque(ID, 1023);
}


void loop()
{
  Estop_engaged = !digitalRead(ESTOP);

  // Estop
  if(Estop_engaged)
  {
    brake_pos = brake_engaged;
  }
  else
  {
    brake_pos = brake_disengaged;
  }
//  ax12a.reset(ID);
  ax12a.move(ID, brake_pos);
//  ax12a.move(ID2, brake_pos);
//  Serial.print("brake_pos");
//  Serial.println(brake_pos);
  
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

//void getSerial()
//{
//  while(Serial.available() > 2)
//  {
//    String msg_type = Serial.readStringUntil('\n');
//    if(msg_type == "motors")
//    {
//      shouldWrite = true;
//      int lMotor_raw = -500;
//      int rMotor_raw = -500;
//      float lBrake_raw = -500;
//      
//      while(msg_type != "END")
//      {
//        msg_type = Serial.readStringUntil('\n');
//        switch(msg_type)
//        {
//          case "lMotor":
//            lMotor_raw = Serial.parseInt();
//            break;
//          case "rMotor":
//            rMotor_raw = Serial.parseInt();
//            break;
//          case "lBrake":
//            lBrake_raw = Serial.parseFloat();
//            break;
//          default:
//            break;
//        }
//        // get motor input
//        
//      }
//      
//      // Process motors
//      if(lMotor_raw != -500)
//      {
//        if( sign(lMotor_raw) == -sign(lMotor) )
//          waitReverse = true;
//        lMotor = map(abs(lMotor_raw), 0, 255, 40, 200);
//        if(lMotor_raw<0)
//          lMotor = -lMotor;
//      }
//      if(rMotor_raw != -500)
//      {
//        if(sign(rMotor_raw) == -sign(rMotor))
//          waitReverse = true;
//        rMotor = map(abs(rMotor_raw), 0, 255, 40, 200);
//        if(rMotor_raw<0)
//          rMotor = -rMotor;
//      }
//      if(lBrake_raw != -500)
//        lBrake = 900 - lBrake_raw*500;
//      
//      lastTime = millis();
//    }
//  }
//  Estop_engaged = !digitalRead(ESTOP);
//}
