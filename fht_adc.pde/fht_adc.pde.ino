/*
  fht_adc.pde
  guest openmusiclabs.com 9.5.12
  example sketch for testing the fht library.
  it takes in data on ADC0 (Analog0) and processes them
  with the fht. the data is sent out over the serial
  port at 115.2kb.  there is a pure data patch for
  visualizing the data.
*/

//#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht
#define LIN_OUT 1
#include <FHT.h> // include the library

void setup() {
  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void loop() {
  while (1) { // reduces jitter
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
    //Serial.println("Start Freq Analysis");
    //Serial.println("==========================================");
    double avg = 0;
    for (int i = 0; i < 128; i++) {
      avg += fht_lin_out[i];
      //Serial.println(fht_lin_out[i]);
    }
    avg /= 128;
    double f = avg - 113;
    f *= 5;
    Serial.println(f);
  }
}

