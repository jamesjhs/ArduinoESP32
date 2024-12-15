/*
Arduino NANO (with deprecated bootloader)


*/

#include "deque.h";

unsigned long timeThen; // time it WILL be when something needs to happen
long randNumber;
int randMax = 100;
int timeDelay = 1000;

// Deque<float> speedVals;
// int maxDequeSize = 20;

void setup() {

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  delay(500);

  for (int i; i < 10; i++) {
    digitalWrite(LED_BUILTIN, 1);
    delay(100);
    digitalWrite(LED_BUILTIN, 0);
    delay(100);
  }  // end LED flash for

  Serial.println("Initialised!");
  timeThen = millis() + timeDelay;

}  // end SETUP()

void loop() {

  if (millis() >= timeThen) {
    timeThen = millis() + timeDelay;
    digitalWrite(LED_BUILTIN, 1);
    delay(100);
    digitalWrite(LED_BUILTIN, 0);


    // queue.push(millis()/33.0);
    // queue.pop();
    // Serial.print("Front: ");
    // Serial.print(queue.front());
    // Serial.print(", Back: ");
    // Serial.println(queue.back());
    
  }

}  // end LOOP()
