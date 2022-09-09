#define SEG_RESET 10

#define PHOTO_PIN 24
#define PHOTO_COVERED LOW

void setup() {
  const int OUTPIN[] = {2, 3, 12, 13, 14, 15, 16, 17, 18, 19};
  for (int i : OUTPIN) {
    pinMode(i, OUTPUT);
  }

  const int INPIN[] = {22, 24, 26};
  for (int i : INPIN) {
    pinMode(i, INPUT);
  }
}

int count = 1;
unsigned long startMillis;

void loop() {
  while (digitalRead(PHOTO_PIN) != PHOTO_COVERED) {
    // 受光状態
    segR(count);
    analogWrite(4, 0);

    if ( (count > 1) && (startMillis + 1000 <= millis()) ) {
      count--;
      startMillis = millis();
    }

    if (digitalRead(PHOTO_PIN) == PHOTO_COVERED) {
      // 離脱処理
      startMillis = millis();
    }
  }

  while (digitalRead(PHOTO_PIN) == PHOTO_COVERED) {
    // 遮光状態
    segR(count);
    analogWrite(4, 100);

    if ( (count < 9) && (startMillis + 1000 <= millis()) ) {
      count++;
      startMillis = millis();
    }

    if (digitalRead(PHOTO_PIN) != PHOTO_COVERED) {
      // 離脱処理
      startMillis = millis();
    }
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

void segR(int num) {
  seg(SEG_RESET);
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  seg(num);
}
