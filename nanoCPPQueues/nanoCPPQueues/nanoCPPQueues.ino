/*
Arduino NANO (with deprecated bootloader)


*/

uint32_t timeNow;
int writeDelay = 2;  // minutes
//int writeDelayMsec = writeDelay * 60 * 1000; // milliseconds
int writeDelayMsec = 500;  // milliseconds
uint32_t writeTime = millis() + writeDelayMsec;
int arraySize = 30;

uint32_t arrTime[30] = {};
int arrSpins[30] = {};
uint32_t arrTimeTemp[30] = { 0 };
int arrSpinsTemp[30] = { 0 };

// unsigned long timeDelay = 1000;
int writeCount = 0;

void setup() {

  Serial.begin(57600);

  pinMode(LED_BUILTIN, OUTPUT);
  delay(500);

  for (int i; i < 10; i++) {
    digitalWrite(LED_BUILTIN, 1);
    delay(100);
    digitalWrite(LED_BUILTIN, 0);
    delay(100);
  }  // end LED flash for

  Serial.println("Initialised!");

}  // end SETUP()

void loop() {
  timeNow = millis();
  if (writeTime < timeNow) {
    writeTime = timeNow + writeDelayMsec;

    Serial.print("writeCount: ");
    Serial.print(writeCount);
    Serial.print(" at millis: ");
    Serial.println(timeNow);

    if (writeCount < arraySize) {

      arrSpins[writeCount] = writeCount;
      arrTime[writeCount] = timeNow;

    } else {
      for (int i; i < arraySize - 1; i++) {

        arrSpinsTemp[i] = arrSpins[i + 1];
        arrSpins[i] = arrSpinsTemp[i];
        arrTimeTemp[i] = arrTime[i + 1];
        arrTime[i] = arrTimeTemp[i];
      }

      arrSpins[arraySize - 1] = writeCount;
      arrTime[arraySize - 1] = timeNow;
    }


    Serial.print("arrSpins: ");
    for (int i : arrSpins) {
      Serial.print(arrSpins[i]);
      Serial.print("    ");
    }
    Serial.println(";");

    Serial.print("arrTime: ");
    for (int i : arrTime) {
      Serial.print(int(arrTime[i]));
      Serial.print(" ");
    }
    Serial.println(";");
    Serial.println("");

    writeCount++;
  }

}  // end LOOP()
