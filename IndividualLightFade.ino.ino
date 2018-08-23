int light = 0; // store the current light value
int RED = 3;
int GREEN = 10;
int BLUE = 11;

void setup() {
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
}

void loop() {
    analogWrite(RED, 255);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 255);
    for (int i = 255; i > 0; i--) {
      analogWrite(RED, i);
      delay(20);
    }
    analogWrite(RED, 255);
    for (int i = 255; i > 0; i--) {
      analogWrite(GREEN, i);
      delay(20);
    }
    analogWrite(GREEN, 255);
    for (int i = 255; i > 0; i--) {
      analogWrite(BLUE, i);
      delay(20);
    }
    analogWrite(RED, 255);
    analogWrite(BLUE, 255);
    analogWrite(GREEN, 255);
    delay(1000);
}
