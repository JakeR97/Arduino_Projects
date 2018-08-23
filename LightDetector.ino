int light = 0; // store the current light value
int RED = 3;
int BLUE = 10;
int GREEN = 11;

void setup() {
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(A0, INPUT);
}

void loop() {
    light = analogRead(A0);
    light = abs((light / 4) - 255);
    analogWrite(RED, light);
    analogWrite(GREEN, light);
    analogWrite(BLUE, light);
}
