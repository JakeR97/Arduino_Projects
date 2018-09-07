
//FHT Variables
#define FHT_N 256 // set to 256 point fht
#define LIN_OUT 1
#include <FHT.h> // include the library
double f;
int color;

//Array used to store RGB values of a spectrum of colors
int colors [102][3];

//Mic related variables
const int sampleWindow = 64; //Sample window in mS (50 mS = 20Hz)
unsigned int sample;

//Identify the pins used to send voltage to RGB wires in the LED strip
int RED = 3;
int GREEN = 5;
int BLUE = 9;


void setup() {
  Serial.begin(115200);

  //  TIMSK0 = 0; // turn off timer0 for lower jitter
  //  ADCSRA = 0xe5; // set the adc to free running mode
  //  ADMUX = 0x40; // use adc0
  //  DIDR0 = 0x01; // turn off the digital input for adc0

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

  // --------------------  Calculate Volume  -------------------- //

  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0; // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  while (1) {
    while (millis() - startMillis < sampleWindow) {
      sample = analogRead(A0);
      if (sample < 1024) {
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

    // ------------------------- Calculate Frequency/Color ------------------//

    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      while (!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fht_input[i] = k; // put real data into bins
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_lin(); // take the output of the fht
    sei();
    Serial.println("Start Freq Analysis");
    //Serial.println("==========================================");
    double avg = 0;
    int slower = FHT_N;
    for (int i = 0; i < slower; i++) {
      avg += fht_lin_out[i];
      //Serial.println(fht_lin_out[i]);
    }
    avg /= slower;
    double f = avg - 113;
    color = f * 5;
    int add = 2000;
    int scale = 50;
    int del = 105;
    color = ((color + add) / scale) % 102;   
    color = color % 102;
    if (color < 0) color *= -1;
    Serial.println(color);

    // --------------------- Write Color to Lights ---------------------//
    delay(del);
    writeColor(color, invVolume);
  }
}

//Due to something weird in the circuit built, analog writing a 0 outputs
//Full power and 255 outputs none, so this function is used to simplify the
//RGB color scheme used
int flipVal(int num) {
  return abs(num - 255);
}

void writeColor(int color, double invVol) {
  analogWrite(RED, (flipVal(colors[color][0] / invVol)));
  analogWrite(GREEN, (flipVal(colors[color][1] / invVol)));
  analogWrite(BLUE, (flipVal(colors[color][2] / invVol)));
}
