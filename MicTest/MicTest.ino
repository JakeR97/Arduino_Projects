   #include <arduinoFFT.h>
   /****************************************
    Example Sound Level Sketch for the
    Adafruit Microphone Amplifier
    ****************************************/
    
    arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
    const int sampleWindow = 32; // Sample window width in mS (32 mS = 20Hz)
    const double signalFrequency = 1000;
    const double samplingFrequency = 5000;
    const uint8_t amplitude = 100;
    
    unsigned int sample;

    double vReal[sampleWindow];
    double vImag[sampleWindow];

    void setup()
    {
      Serial.begin(9600);
    }
     
     
    void loop()
    {
      // --------------------  Calculate Volume  -------------------- //
      
      unsigned long startMillis= millis(); // Start of sample window
      unsigned int peakToPeak = 0; // peak-to-peak level
       
      unsigned int signalMax = 0;
      unsigned int signalMin = 1024;
       
      // collect data for 32 mS
      while (millis() - startMillis < sampleWindow) {
        sample = analogRead(A1);
        if (sample < 1024) { // toss out spurious readings
          int i = (int) (millis() - startMillis);
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
      }
      Serial.println(vReal[sampleWindow]);
      FFT.Windowing(vReal, sampleWindow, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
      FFT.Compute(vReal, vImag, sampleWindow, FFT_FORWARD); /* Compute FFT */
      FFT.ComplexToMagnitude(vReal, vImag, sampleWindow); /* Compute magnitudes */
      double x = FFT.MajorPeak(vReal, sampleWindow, samplingFrequency);
      Serial.println(x);
      
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
