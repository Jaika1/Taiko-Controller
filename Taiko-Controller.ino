/*
 * DEVICE CONFIG
 * 
 * Drum Sensors:
 * LEFTKA   - A0
 * LEFTDON  - A1
 * RIGHTDON - A2
 * RIGHTKA  - A3
 */

 /* TAIKO FORCE PINOUT (As of dec 2021)
  *         ___
  *  |LDO|RKA|RDO|LKA|
  *  |Gnd|Gnd|Gnd|Gnd|
  */

#include <Keyboard.h>

// Writes data viewable by the IDE Serial plotter when true.
bool writeToSerial = true; 
// Between 1 - 1023, lower is more sensetive.
int threshold = 15; 
// Amount above the previously read value from the sensor that is acceptable as a new hit, non-inclusive. (e.g. if 10 was last read and this was 2, then the next value would need to be 13 to register)
int jitter = 2;
// Time in ms to delay key release when a hit is detected. Set to 0 to disable. if on slower hardware, playing without input buffering, or are registering hits inconsistantly despite passing threshold, increase this.
// NOTE: By defualt the leonardo keyboard apparently polls at 125Hz, so theoretical useful min is 8? Would be great to increase this.
int readDelay = 10; 

char* keys;
char keysA[4] = {'h','j','k','l'};

int sensor[4];     // Stores current sensor values
int prevSensor[4]; // Stores previous sensor values
bool rising[4];    // Wether or not the signal rose this hit (to avoid double hits on rising edge)

void setup(){
  if (writeToSerial){
    Serial.begin(9600);
  }
  keys = keysA;
  Keyboard.begin();
}

void loop(){
  updateSensors();
  if (writeToSerial){
    writeSerial();
  }
}

void updateSensors(){
  for (int i = 0; i < 4; i++){
    sensor[i] = analogRead(i);
  }

  int maxKa = sensor[0] > sensor[3] ? 0 : 3;
  int maxDon = sensor[1] > sensor[2] ? 1 : 2;
  int max = sensor[maxKa] > sensor[maxDon] ? maxKa : maxDon;

  if (!rising[max] && sensor[max] > threshold && sensor[max] > prevSensor[max] + jitter){
    Keyboard.press(keys[max]);
    delay(readDelay);
    Keyboard.releaseAll();
  }

  for (int i = 0; i < 4; i++){
    rising[i] = sensor[i] > prevSensor[i] + jitter;
  }

  for (int i = 0; i < 4; i++){
    prevSensor[i] = sensor[i];
  }
}

void writeSerial(){
  if (sensor[0] + sensor[1] + sensor[2] + sensor[3] > 0){
    Serial.print("Ref:1023");                         // Used to keep the plotters graph uniformly scaled to make reading it a tad easier. Disable or tweak if needed for precision. 
    Serial.print(",Thr:"); Serial.print(threshold);   // Threshold for a hit.
    Serial.print(",LKa:"); Serial.print(sensor[0]);   // Left Ka
    Serial.print(",LDo:"); Serial.print(sensor[1]);   // Left Don
    Serial.print(",RDo:"); Serial.print(sensor[2]);   // Right Don
    Serial.print(",RKa:"); Serial.println(sensor[3]); // Right Ka
  }
}