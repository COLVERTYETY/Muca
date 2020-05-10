#include <Muca.h>

Muca muca;

#define CALIBRATION_STEPS 20
short currentCalibrationStep = 0;
unsigned int calibrationGrid[NUM_ROWS * NUM_COLUMNS];

void setup() {
  Serial.begin(115200);
 // muca.skipLine(TX, (const short[]) { 1, 2, 3, 4 }, 4);
//  muca.skipLine(TX, (const short[]) { 18, 19, 20, 21 }, 4);
  // muca.skipLine(RX,(const short[]) {11,12}, 2);
  muca.init();
  muca.useRawData(true); // If you use the raw data, the interrupt is not working
}

void loop() {
  if (muca.updated()) {
  //  SendRawString();
     SendRawByte(); // Faster
  }
  delay(16); // waiting 16ms for 60fps

}

void SendRawString() {
  // Print the array value
  for (int i = 0; i < NUM_ROWS * NUM_COLUMNS; i++) {
    if (muca.grid[i] >= 0) Serial.print(muca.grid[i]); // The +30 is to be sure it's positive
    if (i != NUM_ROWS * NUM_COLUMNS - 1)
      Serial.print(",");
  }
  Serial.println();

}



void SendRawByte() {
  // The array is composed of 254 bytes. The two first for the minimum, the 252 others for the values.
  // HIGH byte minimum | LOW byte minimum  | value 1

  unsigned int minimum = 80000;
  uint8_t rawByteValues[254];

  for (int i = 0; i < NUM_ROWS * NUM_COLUMNS; i++) {
  if (muca.grid[i] > 0 && minimum > muca.grid[i])  {
      minimum = muca.grid[i]; // The +30 is to be sure it's positive
    }
  }
  rawByteValues[0] = highByte(minimum);
  rawByteValues[1] = lowByte(minimum);


  for (int i = 0; i < NUM_ROWS * NUM_COLUMNS; i++) {
    rawByteValues[i + 2] = muca.grid[i] - minimum;
    // Serial.print(rawByteValues[i+2]);
    //  Serial.print(",");

  }
  // Serial.println();
  //GetFPS();
   Serial.write(rawByteValues, 254);
   Serial.flush();
  //Serial.println();
}


char incomingMsg[20];

void serialEvent() {
  int charsRead;
  while (Serial.available() > 0) {
    charsRead = Serial.readBytesUntil('\n', incomingMsg, sizeof(incomingMsg) - 1);
    incomingMsg[charsRead] = '\0';  // Make it a string
    if (incomingMsg[0] == 'g') {
      Gain();
    }
    else if (incomingMsg[0] == 'c') {
      currentCalibrationStep = 0;
    }
  }
}
int frameCount = 0;
float fps = 0.0F;
float t = 0.0F;
float prevtt = 0.0F;

void GetFPS()
{
  frameCount++;
  t += millis() - prevtt;
  if (t > 1000.0f)
  {
    fps = frameCount;
    frameCount = 0;
    t = 0;
  }
  prevtt = millis();
  Serial.println(fps);
  Serial.println(fps);
  Serial.println(fps);
  Serial.println(fps);
}

void Gain() {
  Serial.print("Received:"); Serial.println(incomingMsg);
  char *str;
  char *p = incomingMsg;
  byte i = 0;
  while ((str = strtok_r(p, ":", &p)) != NULL)  // Don't use \n here it fails
  {
    if (i == 1 )  {
      muca.setGain(atoi(str));
    }
    i++;
  }
  incomingMsg[0] = '\0'; // Clear array
}
