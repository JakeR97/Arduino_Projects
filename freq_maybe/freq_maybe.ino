#include <FreqMeasure.h>

void setup() {
   Serial.begin(9600);
   FreqMeasure.begin();
}

double sum = 0;
int count = 0;

void loop() {
  if (FreqMeasure.available()) {
    sum = sum + FreqMeasure.read();
    count++;
    if (count > 30) {
      float freq = FreqMeasure.countToFrequency(sum/count);
      Serial.println(freq);
      sum = 0;
      count = 0;
    }
  }
}
