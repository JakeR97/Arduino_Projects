 /****************************************
    Example Sound Level Sketch for the
    Adafruit Microphone Amplifier
    ****************************************/
     
    const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
    unsigned int sample;
     
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
  
      if (volume > 5) {
        volume = 5;
      } else if (volume < 1) {
        volume = 1;
      }

      double invVolume = abs(volume - 6);

      // ----------------------  Write to lights --------------------- //
      
      Serial.println(invVolume);
    }
