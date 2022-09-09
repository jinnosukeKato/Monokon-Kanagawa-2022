#define SEG_RESET 10

void setup() {
  const int OUTPIN[] = {2, 3, 12, 13, 14, 15, 16, 17, 18, 19};
  for (int i : OUTPIN) {
    pinMode(i, OUTPUT);
  }
}

const int PATTERN[] = {0, 1, 2, 4};
void loop() {
  for (int i : PATTERN) {
    segR(i);
    delay(1000);
  }
}

void seg(int num) {
  const int PTN[][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 1, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}, // 9
    {0, 0, 0, 0, 0, 0, 0}, // RESET 10
  };

  for (int i = 12; i <= 18; i++) {
    digitalWrite(i, PTN[num][i - 12]);
  }
}

void segL(int num) {
  seg(SEG_RESET);
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  seg(num);
}

void segR(int num) {
  seg(SEG_RESET);
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  seg(num);
}
