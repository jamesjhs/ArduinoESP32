/*

LDR is connected between VCC and Pin 21 (PF0), and a variable resistor
between Pin 21 and GND, forming a voltage divider.
LED, via a 1k resistor, is connected between Pin 0 (B0) and GND.
LED lights when LDR threshold is passed, i.e., when the analogue
value at Pin 21 goes higher than 500.
Time is recorded at the initialization using millis(), and also
at each state change (nowOn and nowOff) respective to msec.
Time of nowOn subtracted from time of nowOff then gives the duration.

*/


float pinAna;
long int msec = millis();
long int nowOn;
long int nowOff;
long int time = 0;
int timeShown = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(0, OUTPUT);
  pinMode(11, OUTPUT);  //onboard LED
  pinMode(21, INPUT);

  delay(1000);
  for (int i = 0; i <= 10; i++) {
    digitalWrite(11, HIGH);
    delay(50);
    digitalWrite(11, LOW);
    delay(50);
  }
}

void loop() {
  // delay(50); //debug delay
  pinAna = analogRead(21);

  if (pinAna > 500) {
    digitalWrite(0, HIGH);
    nowOn = millis() - msec;

    if (timeShown == 0) {
      Serial.println(time);
      timeShown = 1;
    }
  } else {
    digitalWrite(0, LOW);
    nowOff = millis() - msec;
    time = nowOff - nowOn;
    timeShown = 0;
  }
}
