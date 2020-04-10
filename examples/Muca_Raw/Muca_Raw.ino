#include <Muca.h>

Muca muca;

void setup() {
  Serial.begin(115200);

  muca.init(); // useInterrupt ne fonctionne pas bien
  muca.useRaw = true;
 // muca.setGain(100);
}

void loop() {
  GetRaw();
}

void GetRaw() {
  if (muca.updated()) {
   for (int i = 0; i < NUM_ROWS * NUM_COLUMNS; i++) {
      if (muca.grid[i] > 0) Serial.print(muca.grid[i]);
      if (i != NUM_ROWS * NUM_COLUMNS - 1)
        Serial.print(",");
    }
   Serial.println();
   
  }
  
}
