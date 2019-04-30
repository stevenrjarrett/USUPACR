// based on the code from the tutorial found here:
// http://forum.arduino.cc/index.php?topic=288234.0

#define BAUDRATE 115200
#define NUMCHARS 32

#define VAR_LMOTOR      0
#define VAR_RMOTOR      1
#define VAR_AUTONOMOUS  2

class motorValueReciever
{
public:

    motorValueReciever(long _baud = BAUDRATE, _numChars = NUMCHARS)
    : baud(_baud),
      newData(false),
      numChars(_numChars),
      receivedChars(new char[numChars]),
      variableName(new char[numChars])
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
                    receivedChars[ndx] = '\0'; // terminate the string
                    recvInProgress = false;
                    ndx = 0;
                    newData = true;
                }
            }

            else if (rc == startMarker) {
                recvInProgress = true;
            }
        }
    }

    void showNewData() {
        if (newData == true) {
            Serial.print("This just in ... ");
            Serial.println(receivedChars);
            newData = false;
        }
    }

    void parseData() {

        // split the data into its parts

      char * strtokIndx; // this is used by strtok() as an index
      int varToWrite = -1;

      strtokIndx = strtok(receivedChars,"=");      // get the first part - the string
      strcpy(variableName, strtokIndx); // copy it to variableName
      if(strcmp(variableName, "lMotor") == 0)
        varToWrite = VAR_LMOTOR;
      else if(strcmp(variableName, "rMotor") == 0)
        varToWrite = VAR_RMOTOR;
      else if(strcmp(variableName, "autonomous") == 0)
        varToWrite = VAR_AUTONOMOUS;

      if(varToWrite != -1)
      {
          strtokIndx = strtok(receivedChars, "\n");
          int value = atoi(strtokIndx);
          switch(varToWrite)
          {
              case VAR_LMOTOR
          }
      }

      strtokIndx = strtok(NULL, "="); // this continues where the previous call left off
      integerFromPC = atoi(strtokIndx);     // convert this part to an integer

    }

private:
    int numChars;
    char *receivedChars;
    bool newData;
    int lMotorValue;
    int rMotorValue;
    long baud;

    char *variableName;
    int integerFromPC = 0;
    float floatFromPC = 0.0;
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
