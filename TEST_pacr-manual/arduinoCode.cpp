// based on the code from the tutorial found here:
// http://forum.arduino.cc/index.php?topic=288234.0

#define BAUDRATE 115200
#define NUMCHARS 128

#define VAR_LMOTOR      0
#define VAR_RMOTOR      1
#define VAR_AUTONOMOUS  2

class motorValueReciever
{
public:

    motorValueReciever(_numChars = NUMCHARS, _verbose = false)
    : newData(false),
      verbose(_verbose),
      numChars(_numChars),
      receivedChars(new char[numChars]),
      variableName(new char[numChars]),
      lMotorValue(0),
      rMotorValue(0),
      autonomous(false),
      newValues(true),
      lastTime(0)
    {
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
    long elapsedTime() { return millis() - lastTime }
    bool hasNewValues() { return newValues; }

    void showNewData() {
        if (newData == true) {
            Serial.print("This just in ... ");
            Serial.println(receivedChars);
        }
    }

    bool update()
    {
        recvWithStartEndMarkers();
        if(newData)
        {
            if(verbose)
                showNewData();
            parseData();
        }
    }


private:
    int numChars;
    bool verbose;
    char *receivedChars;
    bool newData;
    int lMotorValue;
    int rMotorValue;
    bool autonomous;
    bool newValues;

    char *variableName;
    long lastTime;



    void parseData() {
        if(!newData)
            return;
        // split the data into its parts
        bool eos = false;

        char * strtokIndx; // this is used by strtok() as an index
        int varToWrite = -1;

        strtokIndx = strtok(receivedChars,"=");      // get the first part

        while(!eos)
        {
            strcpy(variableName, strtokIndx); // copy it to variableName
            if(strcmp(variableName, "lMotor") == 0)
                varToWrite = VAR_LMOTOR;
            else if(strcmp(variableName, "rMotor") == 0)
                varToWrite = VAR_RMOTOR;
            else if(strcmp(variableName, "autonomous") == 0)
                varToWrite = VAR_AUTONOMOUS;
            else if(strcmp(variableName, "end") == 0)
                eos = true;

//            if(eos)
//                break;
            if(varToWrite != -1)
            {
                strtokIndx = strtok(NULL, "\n");
                int value = atoi(strtokIndx);
                switch(varToWrite)
                {
                case VAR_LMOTOR:
                    lMotorValue = value;
                    break;
                case VAR_RMOTOR:
                    rMotorValue = value;
                    break;
                case VAR_AUTONOMOUS:
                    autonomous = (value==0) ? false : true;
                    break;
                }
            }
            if(!eos)
                strtokIndx = strtok(NULL,"=");      // get the next part
        }
        newValues = true;
        newData   = false;
    }

    void recvWithStartEndMarkers() {
        static boolean recvInProgress = false;
        static byte ndx = 0;
        char startMarker = '<';
        char endMarker = '>';
        char rc;

     // if (Serial.available() > 0) {
        while (Serial.available() > 0 && newData == false) {
            rc = Serial.read();

            if (recvInProgress == true) {
                if (rc != endMarker) {
                    receivedChars[ndx] = rc;
                    ndx++;
                    if (ndx >= numChars) {
                        ndx = numChars - 1;
                    }
                }
                else {

                    receivedChars[ndx] = 'e'; // terminate the string
                    receivedChars[(ndx+1)%numChars] = 'n'; // terminate the string
                    receivedChars[(ndx+2)%numChars] = 'd'; // terminate the string
                    receivedChars[(ndx+3)%numChars] = '\0'; // terminate the string
                    recvInProgress = false;
                    ndx = 0;
                    newData = true;
                    lastTime = millis();
                }
            }

            else if (rc == startMarker) {
                recvInProgress = true;
            }
        }
    }
};







//#define DELAYTIME      200
#define ESTOP          2
#define LMOTOR         9
#define RMOTOR         10
#define LMOTOR_REVERSE 7
#define RMOTOR_REVERSE 8
#define AUTONOMOUS     3

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
    Serial.begin(115200);
    Serial.println("<Arduino is ready>");
}


void loop() {
    motors.update();
    if(motors.elapsedTime() < 500)
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
                digitalWrite(AUTONOMOUS, autonomous)
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
            analogWrite(LMOTOR, abs(lMotor);
            analogWrite(RMOTOR, abs(rMotor);
            digitalWrite(AUTONOMOUS, autonomous);

            if(VERBOSE && (millis() - lastSend > 1000))
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
        }
    }
}




void showParsedData() {
 Serial.print("Message ");
 Serial.println(variableName);
 Serial.print("Integer ");
 Serial.println(integerFromPC);
 Serial.print("Float ");
 Serial.println(floatFromPC);
}
