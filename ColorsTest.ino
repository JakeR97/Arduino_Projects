//Identify the pins used to send voltage to RGB wires in the LED strip
int RED = 3;
int GREEN = 5;
int BLUE = 9;

//Array used to store RGB values of a spectrum of colors
int colors [102][3];

void setup() {
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
    writeColor(i);
    delay(100);
  }
}

int FlipVal(int num) {
  return abs(num - 255);
}

void writeColor(int color) {
  analogWrite(RED, FlipVal(colors[color][0]));
  analogWrite(GREEN, FlipVal(colors[color][1]));
  analogWrite(BLUE, FlipVal(colors[color][2]));
}



