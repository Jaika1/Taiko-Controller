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
bool writeToSerial = false;
// Serial write threshold = 0: Any value above 0 | 1: Any value above 'threshold' | 2: always 
uint8_t serialWriteThreshold = 0; 
// Between 1 - 1023, lower is more sensetive.
int thresholdBase = 13; 
int threshold[4] = { 15, 15, 15, 15 }; // LK, LD, RD, RK
// Amount above the previously read value from the sensor that is acceptable as a new hit, non-inclusive. (e.g. if 10 was last read and this was 2, then the next value would need to be 13 to register)
int jitter = 0;
// Time in ms to delay key release when a hit is detected. Set to 0 to disable. if on slower hardware, playing without input buffering, or are registering hits inconsistantly despite passing threshold, increase this.
// NOTE: By defualt the leonardo keyboard apparently polls at 125Hz, so theoretical useful min is 8? Would be great to increase this.
int keyDuration = 22; 

char* keys;
char keysA[4] = {'h','j','k','l'};

int sensor[4];     // Stores current sensor values
int prevSensor[4]; // Stores previous sensor values
bool rising[4];    // Wether or not the signal rose this hit (to avoid double hits on rising edge)

void setup(){
  // ADC polling to 1Mhz
  ADCSRA=(ADCSRA&0xF80)|0x04;

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
  int thres = thresholdBase + threshold[max];

  if (!rising[max] && sensor[max] > thres && sensor[max] > prevSensor[max]){
    Keyboard.press(keys[max]);
    delay(keyDuration);
    Keyboard.releaseAll();
  }

  for (int i = 0; i < 4; i++){
    rising[i] = (sensor[i] > prevSensor[i] + jitter) && sensor[i] > threshold[i];
  }

  for (int i = 0; i < 4; i++){
    prevSensor[i] = sensor[i];
  }
}

void writeSerial(){
  bool doPrint;
  switch(serialWriteThreshold){
    case 0:
      doPrint = sensor[0] + sensor[1] + sensor[2] + sensor[3] > 0;
    break;
    case 1:
      doPrint = sensor[0] > threshold[0] || sensor[1] > threshold[1] || sensor[2] > threshold[2] || sensor[3] > threshold[3];
    break;
    case 2:
      doPrint = true;
    break;
    default:
      doPrint = false;
    break;
  }

  if (doPrint){
    Serial.print("Ref:1023");                         // Used to keep the plotters graph uniformly scaled to make reading it a tad easier. Disable or tweak if needed for precision. 
    Serial.print(",ThrB:"); Serial.print(thresholdBase);   // Threshold base
    Serial.print(",LKa:"); Serial.print(sensor[0]);   // Left Ka
    Serial.print(",LDo:"); Serial.print(sensor[1]);   // Left Don
    Serial.print(",RDo:"); Serial.print(sensor[2]);   // Right Don
    Serial.print(",RKa:"); Serial.println(sensor[3]); // Right Ka
  }
}