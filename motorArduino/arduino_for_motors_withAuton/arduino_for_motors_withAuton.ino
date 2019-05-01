
// based on the code from the tutorial found here:
// http://forum.arduino.cc/index.php?topic=288234.0

#define NUMCHARS 128

#define VAR_LMOTOR      0
#define VAR_RMOTOR      1
#define VAR_AUTONOMOUS  2

int sign(int val) { return (val<0) ? -1 : 1; }
float sign(float val) { return (val<0) ? -1 : 1; }

class motorValueReciever
{
public:
    bool verbose;

    motorValueReciever( char _endMarker = '\n', int _numChars = NUMCHARS, bool _verbose = false)
    : newData(false),
      verbose(_verbose),
      numChars(_numChars),
      receivedChars(new char[numChars]),
      variableName(new char[numChars]),
      lMotorValue(0),
      rMotorValue(0),
      autonomous(false),
      newValues(true),
      lastTime(0),
      endMarker(_endMarker)
    {
        receivedChars[numChars-1] = '\0';
//        Serial.begin(baud);
//        Serial.println("<Arduino is ready>");
    }

    ~motorValueReciever()
    {
        delete[] receivedChars;
        delete[] variableName;
//        Serial.end();
    }

    int lMotor() { newValues = false; return lMotorValue; }
    int rMotor() { newValues = false;  return rMotorValue; }
    int autonomousEngaged() { newValues = false;  return autonomous; }
    long getLastTime() { return lastTime; }
    long elapsedTime() { return millis() - lastTime; }
    bool hasNewValues() { return newValues; }

    void showNewData()
    {
        if (newData == true) {
            Serial.print("This just in ... ");
            Serial.println(receivedChars);
        }
    }

    bool update()
    {
        recvWithEndMarker();
        if(newData)
        {
            if(verbose)
                showNewData();
            parseData();
        }
    }


private:
    int numChars;
    char *receivedChars;
    bool newData;
    int lMotorValue;
    int rMotorValue;
    bool autonomous;
    bool newValues;

    char *variableName;
    long lastTime;
    char endMarker;



    void parseData() {
        static int varToWrite = VAR_LMOTOR;
        if(!newData)
            return;
        // Parse a value
        int value = atoi(receivedChars);
        switch(varToWrite)
        {
        case VAR_LMOTOR:
            lMotorValue = value;
            varToWrite = VAR_RMOTOR;
            break;
        case VAR_RMOTOR:
            rMotorValue = value;
            varToWrite = VAR_LMOTOR;
            newValues = true;
            lastTime = millis();
            break;
//        case VAR_AUTONOMOUS:
//            autonomous = (value==0) ? false : true;
//            break;
        }

        newData   = false;
    }


    void recvWithEndMarker()
    {
        static byte ndx = 0;
        char rc;

        while (Serial.available() > 0 && newData == false)
        {
            rc = Serial.read();

            if (rc != endMarker)
            {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars)
                {
                    ndx = numChars - 1;
                }
            }
            else
            {
                receivedChars[ndx] = '\0'; // terminate the string
                ndx = 0;
                newData = true;
            }
        }
    }
};

























//#define DELAYTIME      200
#define BAUDRATE       115200
#define ESTOP          2
#define LMOTOR         9
#define RMOTOR         10
#define LMOTOR_REVERSE 7
#define RMOTOR_REVERSE 8
#define AUTONOMOUS     3
#define SEND_DELAY     1000

#define VERBOSE true

#define MAX_INACTIVE_TIME 500 // ms
#define REVERSE_PAUSE_TIME 100 // ms

int lMotor = 0;
int rMotor = 0;
bool newMotorValues = true;
bool autonomous = false;
long pauseUntil = 0;
long lastSend = 0;
motorValueReciever motors;

void setup() {
    Serial.begin(BAUDRATE);
    Serial.println("<Arduino is ready>");
    pinMode(LMOTOR, OUTPUT);
    pinMode(RMOTOR, OUTPUT);
    pinMode(LMOTOR_REVERSE, OUTPUT);
    pinMode(RMOTOR_REVERSE, OUTPUT);
    pinMode(ESTOP, INPUT);
    analogWrite(LMOTOR, 0);
    analogWrite(RMOTOR, 0);
    digitalWrite(LMOTOR_REVERSE,false);
    digitalWrite(RMOTOR_REVERSE,false);
//      delay(100);
}


void loop() {
    motors.update();
    bool Estop_engaged = !(digitalRead(ESTOP));
    long tm = motors.elapsedTime();

    // Parse motor information and set appropriate pin values.
    if(tm < MAX_INACTIVE_TIME && !Estop_engaged)
    {
        if(motors.hasNewValues())
        {
            int lMotor_raw = motors.lMotor();
            int rMotor_raw = motors.rMotor();
            autonomous = motors.autonomousEngaged();
            if(sign(lMotor_raw) == -sign(lMotor) || sign(rMotor_raw) == -sign(rMotor))
            {
                analogWrite(LMOTOR, 0);
                analogWrite(RMOTOR, 0);
                digitalWrite(LMOTOR_REVERSE, lMotor_raw < 0);
                digitalWrite(RMOTOR_REVERSE, rMotor_raw < 0);
                digitalWrite(AUTONOMOUS, autonomous);
                pauseUntil = millis() + REVERSE_PAUSE_TIME;
            }
            if(abs(lMotor_raw) > 0)
              lMotor = map(abs(lMotor_raw), 0, 255, 30, 200);
            else
              lMotor = 0;
            if(abs(rMotor_raw) > 0)
              rMotor = map(abs(rMotor_raw), 0, 255, 30, 200);
            else
              rMotor = 0;
            if(lMotor_raw<0) lMotor = -lMotor;
            if(rMotor_raw<0) rMotor = -rMotor;
            newMotorValues = true;
        }

        if(millis() > pauseUntil && newMotorValues)
        {
            digitalWrite(LMOTOR_REVERSE,lMotor<0);
            digitalWrite(RMOTOR_REVERSE,rMotor<0);
            analogWrite(LMOTOR, abs(lMotor));
            analogWrite(RMOTOR, abs(rMotor));
            digitalWrite(AUTONOMOUS, autonomous);

            if(VERBOSE && (millis() - lastSend > SEND_DELAY))
            {
              Serial.print(" lMotor = ");
              Serial.print(lMotor);
              Serial.print(", rMotor = ");
              Serial.print(rMotor);
              Serial.print(", EStop = ");
              Serial.print(Estop_engaged);
              Serial.print(", autonomous = ");
              Serial.println(autonomous);
              lastSend = millis();
            }

            newMotorValues = false;
        }
    }
    else
    {
        analogWrite(LMOTOR, 0);
        analogWrite(RMOTOR, 0);
        if(VERBOSE && (millis() - lastSend > SEND_DELAY))
        {
            Serial.print("Setting to 0. Estop = ");
            Serial.print(Estop_engaged);
//            Serial.print(", elapsed time = ");
//            Serial.print(tm);
//            Serial.print("/");
//            Serial.print(MAX_INACTIVE_TIME);
            Serial.print("\n");
            lastSend = millis();
        }
    }

    //delay(DELAYTIME);
}
