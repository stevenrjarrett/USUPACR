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

    int lMotor() { return lMotorValue; }
    int rMotor() { return rMotorValue; }
    int autonomousEngaged() { return autonomous; }
    long getLastTime() { return lastTime; }

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











// simple parse demo
char receivedChars[] = "This is a test, 1234, 45.3" ;

char variableName[32] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;

char recvChar;
char endMarker = '>';
boolean newData = false;


void setup() {
 Serial.begin(9600);
 Serial.println("<Arduino is ready>");

 parseData();
 showParsedData();
}


void loop() {

}




void showParsedData() {
 Serial.print("Message ");
 Serial.println(variableName);
 Serial.print("Integer ");
 Serial.println(integerFromPC);
 Serial.print("Float ");
 Serial.println(floatFromPC);
}
