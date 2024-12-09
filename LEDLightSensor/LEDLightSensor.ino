/*

Teensy 2.0 board

4-bit 7-segment LED display with decimal points
https://www.instructables.com/12-Pin-7-Segment-Display-Wiring-Tutorial/

Looking down on chip, Pin 1 is bottom left.  
Anodes = Pins 12, 9, 8, 6
Segments = A 11, B 7, C 4, D 2, E 1, F 10, G 5
Decimal point = Pin 19

Connect ANODES via resistors to pins 1, 2, 3, 4, respectively.
Connect segment CATHODES to pins 12 to 18 respectively
Connect decimal point cathode to pin 19.

Selecting anode HIGH and cathode LOW will light a segment.

Number Display

Number | A | B | C | D | E | F | G |
------------------------------------
    0  | x | x | x | x | x | x |   |
    1  |   | x | x |   |   |   |   |
    2  | x | x |   | x | x |   | x |
    3  | x | x | x | x |   |   | x |
    4  |   | x | x |   |   | x | x |
    5  | x |   | x | x |   | x | x |
    6  | x |   | x | x | x | x | x |
    7  | x | x | x |   |   |   |   |
    8  | x | x | x | x | x | x | x |
    9  | x | x | x | x |   | x | x |

*/

int anodePins[] = { 1, 2, 3, 4 };
int segmentPins[] = { 12, 13, 14, 15, 16, 17, 18, 19 };
int startLDRpin = 21;
int stopLDRpin = 20;
float valStartLDR;
float valStopLDR;
int triggerVal = 500;
int timerRun = 0;
unsigned long timeStart;
unsigned long timeNow;
unsigned long timeStop = 0.000;
float speed;
float LDRdistance = 10;     // cm
float cms2mph = 0.0223694;  // cm per sec in mph
float scale = 76.0;
float speedcmsec;

//      #  = {a,b,c,d,e,f,g,.};

int number[10][7] = {

  { 1, 1, 1, 1, 1, 1, 0 },  // number 0
  { 0, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 0, 1, 1, 0, 1 },
  { 1, 1, 1, 1, 0, 0, 1 },
  { 0, 1, 1, 0, 0, 1, 1 },
  { 1, 0, 1, 1, 0, 1, 1 },
  { 1, 0, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 1, 1 }  // number 9

};  // end array

float timeValue = 0.000;

void setup() {

  pinMode(startLDRpin, INPUT);  //topLDR - start
  pinMode(stopLDRpin, INPUT);   //bottomLDR - stop

  // Anodes

  for (int i = 0; i < 4; i++) {
    pinMode(anodePins[i], OUTPUT);
  }

  // Cathodes

  for (int i = 0; i < 8; i++) {  //8th pin is the decimal point
    pinMode(segmentPins[i], OUTPUT);
  }

  delay(500);

  for (int k = 0; k < 4; k++) {
    digitalWrite(anodePins[k], LOW);
  }

  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], HIGH);
    delay(50);
    digitalWrite(segmentPins[i], LOW);

    delay(50);
    digitalWrite(segmentPins[7], HIGH);
    digitalWrite(segmentPins[7], LOW);
  }
  for (int k = 0; k < 4; k++) {
    digitalWrite(anodePins[k], HIGH);
  }
}  // end setup loop

void displayChar(int input, int position, int totalLength, int dotPosition) {

  if (position > dotPosition) {
    position = position - 1;
  }

  if (position <= 3) {
    digitalWrite(anodePins[position], LOW);

    for (int j = 0; j <= 9; j++) {
      for (int i = 0; i < 7; i++) {

        // digitalWrite(segmentPins[i], LOW);
        // digitalWrite(segmentPins[7], LOW);
        digitalWrite(segmentPins[i], number[input][i]);
        if (position == dotPosition - 1) {
          digitalWrite(segmentPins[7], HIGH);
        } else {
          digitalWrite(segmentPins[7], LOW);
        }
      }
    }
    //digitalWrite(anodePins[position], HIGH);
  }
  // delay(50);
  for (int i = 0; i < 4; i++) {
    digitalWrite(anodePins[i], HIGH);
    for (int j = 0; j < 8; j++) {
      digitalWrite(segmentPins[j], LOW);
    }
  }

}  //end displayChar function

void sendToDisplay(float input) {
  /* 
Takes the raw value as a float, and converts it to a string 
so that each character can be sent across to the displayChar function.
The decimal point position is noted and sent separately. 

*/
  String strFromInput = String(input);
  int lenInputString = strFromInput.length();
  int dotPosition = String(input).indexOf(".");

  for (int i = 0; i < lenInputString; i++) {
    String stringDigit = strFromInput[i];
    int intStringDigit = stringDigit.toInt();
    // displayChar(int number, int position, int totalLength, int dotPosition)

    if (i != dotPosition) {
      displayChar(intStringDigit, i, lenInputString - 1, dotPosition);
    }

  }  // end length for loop

}  // end sendToDisplay function

void loop() {

  // READING VALUES FROM THE LDRs SECTION //

  valStartLDR = analogRead(startLDRpin);
  valStopLDR = analogRead(stopLDRpin);

  if (valStartLDR < 500) {
    if (valStopLDR < 500) {
    } else if (timerRun == 0) {

      timerRun = 1;
      timeStart = millis();
      Serial.println("Timer triggered");  // only change if not that value
    }
  }

  if (valStopLDR < 500) {
    if (timerRun == 1) {
      timeStop = millis() - timeStart;
      timerRun = 0;
      Serial.println("Timer stopped");
      /*
To calculate speeds: 
 D = S * T
 S = D / T
Start with time taken in msec for a model train to get 10cm
Divide by 1000.00 to get in seconds
Multiply by the scale factor (76) to assume distance is scaled up
Result = cm/sec
Multiply by 0.022369 to get into mph.

*/
      speedcmsec = (LDRdistance * scale) / (timeStop / 1000.000);
      Serial.print("Time taken (sec): ");
      Serial.println(timeStop / 1000.000);
      Serial.print("Speed in cm/sec: ");
      Serial.println(speedcmsec);
      speed = speedcmsec * cms2mph;
      Serial.print("Speed in mph: ");
      Serial.println(speed);
    }
  }

  if (timerRun == 1) {
    sendToDisplay((millis()-timeStart)/1000.000);
  } else {

    sendToDisplay(speed);
  }  //sends the value as a raw number to sendToDisplay function

}  // end main loop
