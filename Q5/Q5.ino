#define SEG_RESET 10
#define SEG_R 11

#define SW_PIN 22
#define SW_ON HIGH

void setup() {
  const int OUTPIN[] = {2, 3, 5, 12, 13, 14, 15};
  for (int i : OUTPIN) {
    pinMode(i, OUTPUT);
  }

  const int INPIN[] = {22, 24, 26};
  for (int i : INPIN) {
    pinMode(i, INPUT);
  }
  // init end
}

void loop() {
  if (digitalRead(SW_PIN) == SW_ON) {
    // 押されているとき
    digitalWrite(16, HIGH);
    digitalWrite(17, LOW);
    clock();
  } else {
    // 押されていないとき
    digitalWrite(16, HIGH);
    digitalWrite(17, HIGH);
    stepL();
  }
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

void stepL() {
  for (int i = 0; i < 4; i++) {
    step(i);
  }
}

void stepR() {
  for (int i = 3; i >= 0; i--) {
    step(i);
  }
}
