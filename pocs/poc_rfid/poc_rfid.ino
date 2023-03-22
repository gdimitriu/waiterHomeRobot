#include <Servo.h>
#include <Arduino.h>
#include <EnableInterrupt.h>
#define SERVO_PIN 9
Servo servo;

#define SENSOR_PIN 4
#define LOWER_SWITCH 5
#define UPPER_SWITCH 6

char inData[20]; // Allocate some space for the string
char inChar; // Where to store the character read
byte index = 0; // Index into array; where to store the character

//0 none -1 lower 1 upper
volatile int switchTriggered;

void upperDirStopServo() {
  servo.detach();
  switchTriggered = 1;
  Serial.print("Upper trigger ");Serial.print(digitalRead(UPPER_SWITCH));
}

void lowerDirStopServo() {
  servo.detach();
  switchTriggered = -1;
  Serial.print("Lower trigger ");Serial.print(digitalRead(LOWER_SWITCH));
}

void sensorTriggered() {
  if ( switchTriggered == -1 ) {
     servo.attach(SERVO_PIN);
     servo.write(180);
     switchTriggered = 0;
     Serial.print("Sensor trigger ");
  }
}

void setup() {
  Serial.begin(9600);
  switchTriggered = 0;
  pinMode(LOWER_SWITCH, INPUT_PULLUP);
  pinMode(UPPER_SWITCH, INPUT_PULLUP);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  enableInterrupt(LOWER_SWITCH, lowerDirStopServo, FALLING);
  enableInterrupt(UPPER_SWITCH, upperDirStopServo, FALLING);
  enableInterrupt(SENSOR_PIN, sensorTriggered, FALLING);
}

static void makeCleanup() {
  for (index = 0; index < 20; index++) {
    inData[index] = '\0';
  }
  index = 0;
  inChar ='0';
}

static boolean isValidNumber(char *data)
{
  if (strlen(data) == 0 ) return false;
   if(!(data[0] == '+' || data[0] == '-' || isDigit(data[0]))) return false;

   for(byte i=1;i<strlen(data);i++) {
       if(!(isDigit(data[i]) || data[i] == '.')) return false;
   }
   return true;
}

void makeMove(char *data) {
  char *realData = data;
  if (strlen(realData) > 0) {
    realData[strlen(realData) -1] = '\0';
  } else {
    return;
  }
  if (strlen(realData) == 1) {
    if (realData[0] == 'l') {
      if ( switchTriggered == -1 )
        return;
      Serial.println("Lower servo");
      servo.attach(SERVO_PIN);
      servo.write(0);
      switchTriggered = 0;
    } else if ( realData[0] == 'u' ) {
      if ( switchTriggered == 1 )
        return;
      Serial.println("Upper servo");
      servo.attach(SERVO_PIN);
      servo.write(180);
      switchTriggered = 0;
    } else if ( realData[0] == 's' ) {
      
      servo.detach();
      Serial.println("Stop servo");
      switchTriggered = 0;
    }
  }
}

void receiveCommand() {
  while(Serial.available() > 0) {
     if(index < 19) // One less than the size of the array
     {
        inChar = Serial.read(); // Read a character
        if (inChar == '\r' || inChar == '\n' || inChar == '\0' || inChar < 35 || inChar > 122) {
          continue;
        }
        //commands start with a letter capital or small
        if (index == 0 && !((inChar > 64 && inChar < 91) || (inChar > 96 && inChar < 123))) {
          continue;
        }
        inData[index++] = inChar; // Store it
        inData[index] = '\0'; // Null terminate the string
     } else {
        makeCleanup();
     }
  }
  if (inData[index-1] == '#') {
    makeMove(inData);
    makeCleanup();
  }
}
void loop() {
  receiveCommand();
}
