int RED = 3;
int GREEN = 5;
int BLUE = 9;

void setup() {
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
}

void loop() {
  analogWrite(RED, FlipVal(255));
  analogWrite(GREEN, FlipVal(255));
  analogWrite(BLUE, FlipVal(0));
}

int FlipVal(int num) {
  return abs(num-255);
}

