#define SEG_RESET 10
#define SEG_R 11

void setup() {
  const int OUTPIN[] = {2, 3, 4, 5, 16, 17};
  for (int i : OUTPIN) {
    pinMode(i, OUTPUT);
  }
  // init end
}

void loop() {
  // 回転
  digitalWrite(16, LOW);
  digitalWrite(17, HIGH);
  clock();
  delay(2000);

  // 停止
  digitalWrite(16, HIGH);
  digitalWrite(17, HIGH);
  clock();

  // 鳴音
  analogWrite(4, 100);
  delay(1000);
  analogWrite(4, 0);
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
