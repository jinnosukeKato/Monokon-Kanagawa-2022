#define SEG_RESET 10
#define SEG_R 11

void setup() {
  const int OUTPIN[] = {2, 3, 5, 12, 13, 14, 15};
  for (int i : OUTPIN) {
    pinMode(i, OUTPUT);
  }
  // init end
}

void loop() {
  stepL(360);
  stepR(180);
}

void clock(int ms) {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);

  digitalWrite(5, HIGH);
  delay(ms);
  digitalWrite(5, LOW);
}

void clock() {
  clock(5);
}

void step(int n) {
  const int PTN[4][4] = {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 0, 0, 1}
  };

  for (int i = 12; i <= 15; i++) {
    digitalWrite(i, PTN[n][i - 12]);
  }
  clock();
}

void stepL(int angle) {
  for (int i = 0; i <= angle; i += 12) {
    for (int n = 0; n < 4; n++) {
      step(n);
    }
  }
}

void stepR(int angle) {
  for (int i = 0; i <= angle; i += 12) {
    for (int n = 3; n >= 0; n--) {
      step(n);
    }
  }
}
