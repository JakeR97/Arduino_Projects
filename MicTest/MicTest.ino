#include <arduinoFFT.h>
#include <FreqMeasure.h>
#include <FFT.h>

#define FFT_N 256;
/****************************************
  Example Sound Level Sketch for the
  Adafruit Microphone Amplifier
 ****************************************/

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
const int sampleWindow = 128; // Sample window width in mS (32 mS = 20Hz)
const double signalFrequency = 1000;
const double samplingFrequency = 5000;
const uint8_t amplitude = 100;

unsigned int sample;

double vReal[sampleWindow];
double vImag[sampleWindow];

void setup()
{
  Serial.begin(9600);
  FreqMeasure.begin();
}

double sum = 0;
int count = 0;

void loop()
{

  FFT = arduinoFFT();
  memset(vReal, 0, sizeof(vReal));
  memset(vImag, 0, sizeof(vImag));
  // --------------------  Calculate Volume  -------------------- //

  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0; // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 32 mS
  int i = 0;
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(A0);
    if (sample < 1024) { // toss out spurious readings
      vReal[i] = sample;
      Serial.print(i);
      Serial.print(" ");
      Serial.println(vReal[i]);
      if (sample > signalMax) {
        signalMax = sample; // save just the max levels
      }
      else if (sample < signalMin) {
        signalMin = sample; // save just the min levels
      }
    }
    i++;
  }
  Serial.println(vReal[sampleWindow]);
  FFT.Windowing(vReal, sampleWindow, FFT_WIN_TYP_RECTANGLE, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(vReal, vImag, sampleWindow, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, sampleWindow); /* Compute magnitudes */
  double x = FFT.MajorPeak(vReal, sampleWindow, samplingFrequency);
  Serial.println("Frequency Polling");
  Serial.println("=============================================");
  double avg = 0;
  for (int i = 10; i < sampleWindow; i++) {
    avg += x;
    double x = vReal[i];
    //Serial.println(x);
  }
  avg /= sampleWindow;
  Serial.println("Avg Freq");
  Serial.println(avg / 4);

  peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
  double volume = (peakToPeak * 5.0) / 256; // convert to volts

  if (volume > 5) {
    volume = 5;
  } else if (volume < 1) {
    volume = 1;
  }
  double invVolume = abs(volume - 6);

  // ----------------------  Write to lights --------------------- //


  Serial.println(invVolume);
}
long getFrequency() {
#define SAMPLES 32;
  long freq = 0;
  for (int j = 0; j < SAMPLES; j++) freq += 500000 / pulseIn(
  }

}

