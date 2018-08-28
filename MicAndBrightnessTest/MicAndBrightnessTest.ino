#include <arduinoFFT.h>

 
#define SAMPLES 128             //Must be a power of 2
#define SAMPLING_FREQUENCY 1000 //Hz, must be less than 10000 due to ADC

arduinoFFT FFT = arduinoFFT();

double vReal[SAMPLES];
double vImag[SAMPLES];

unsigned int sampling_period_us;
unsigned long microseconds;

//Identify the pins used to send voltage to RGB wires in the LED strip
int RED = 3;
int GREEN = 5;
int BLUE = 9;

//Array used to store RGB values of a spectrum of colors
int colors [102][3];

//Mic related variables
const int sampleWindow = 50; //Sample window in mS (50 mS = 20Hz)
unsigned int sample;

void setup() {
  Serial.begin(9600);

  sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
  
  //Initialize RGB pins
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  //Initialize color spectrum array
  for (int i = 0; i < 102 ; i++) {
    //RED to YELLOW
    if (i < 17)
    {
      colors[i][0] = 255;
      colors[i][1] = i * 15;
      colors[i][2] = 0;
    }
    //YELLOW to GREEN
    else if (i < 34) {
      colors[i][0] = 255 - ((i % 17) * 15);
      colors[i][1] = 255;
      colors[i][2] = 0;
    }
    //GREEN to TEAL
    else if (i < 51) {
      colors[i][0] = 0;
      colors[i][1] = 255;
      colors[i][2] = (i % 17) * 15;
    }
    //TEAL to BLUE
    else if (i < 68) {
      colors[i][0] = 0;
      colors[i][1] = 255 - ((i % 17) * 15);
      colors[i][2] = 255;
    }
    //BLUE to PURPLE
    else if ( i < 85) {
      colors[i][0] = (i % 17) * 15;
      colors[i][1] = 0;
      colors[i][2] = 255;
    }
    //PURPLE to RED
    else {
      colors[i][0] = 255;
      colors[i][1] = 0;
      colors[i][2] = 255 - ((i % 17) * 15);
    }
  }
}

void loop() { 
 

  for (int i = 0; i < 102; i++) {

    // --------------------  Calculate Volume  -------------------- //
      
      unsigned long startMillis= millis(); // Start of sample window
      unsigned int peakToPeak = 0; // peak-to-peak level
       
      unsigned int signalMax = 0;
      unsigned int signalMin = 1024;

      
       
      // collect data for 50 mS
      while (millis() - startMillis < sampleWindow) {
        sample = analogRead(A1);
        if (sample < 1024) { // toss out spurious readings
          if (sample > signalMax) {
            signalMax = sample; // save just the max levels
          }
          else if (sample < signalMin) {
            signalMin = sample; // save just the min levels
          }
        }
      }
      
      peakToPeak = signalMax - signalMin; // max - min = peak-peak amplitude
      double volume = (peakToPeak * 5.0) / 256; // convert to volts
  
      if (volume > 10) {
        volume = 10;
      } else if (volume < 1) {
        volume = 1;
      }

      double invVolume = abs(volume - 11);
      Serial.println(invVolume);

    // --------------------- Write Color to Lights ---------------------//

    
      writeColor(50, invVolume);
      
      delay(10);
  }
   
}

//Due to something weird in the circuit built, analog writing a 0 outputs
//Full power and 255 outputs none, so this function is used to simplify the
//RGB color scheme used
int flipVal(int num) {
  return abs(num - 255);
}


void fft() {
  FF
}

void writeColor(int color, double invVol) {
  analogWrite(RED, (flipVal(colors[color][0]/invVol)));
  analogWrite(GREEN, (flipVal(colors[color][1]/invVol)));
  analogWrite(BLUE, (flipVal(colors[color][2]/invVol)));
}

