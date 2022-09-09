#define SEG_RESET 10
#define SEG_L 11
#define SEG_M 12
#define SEG_R 13

#define SW_PIN 22
#define SW_ON HIGH
#define VR_PIN A2


void setup() {
  const int OUTPIN[] = {2, 3, 12, 13, 14, 15, 16, 17, 18, 19};
  for (int i : OUTPIN) {
    pinMode(i, OUTPUT);
  }

  const int INPIN[] = {SW_PIN, VR_PIN};
  for (int i : INPIN) {
    pinMode(i, INPUT);
  }
}

boolean motorFlag = false;
boolean stepFlag = false;
void loop() {
  if (analogRead(VR_PIN) < 400) {
    // 左
    segR(SEG_L);

    while (digitalRead(SW_PIN) == SW_ON) {
      segR(SEG_L);
      if (digitalRead(SW_PIN) != SW_ON) {
        motorFlag = !motorFlag;
        if (motorFlag) {
          digitalWrite(16, LOW);
          digitalWrite(17, HIGH);
        } else {
          digitalWrite(16, HIGH);
          digitalWrite(17, HIGH);
        }
        clockSegR(SEG_L);
      }
    }
  } else if (analogRead(VR_PIN) > 600) {
    // 右
    if (stepFlag) {
      stepRSegR(SEG_R);
    } else {
      segR(SEG_R);
    }

    while (digitalRead(SW_PIN) == SW_ON) {
      if (stepFlag) {
        stepRSegR(SEG_R);
      } else {
        segR(SEG_R);
      }
      if (digitalRead(SW_PIN) != SW_ON) {
        stepFlag = !stepFlag;
      }
    }
  } else {
    // 中央付近
    segR(SEG_M);
    if (stepFlag)
      stepFlag = false;

    if (motorFlag) {
      motorFlag = false;
      digitalWrite(16, HIGH);
      digitalWrite(17, HIGH);
      clockSegR(SEG_M);
    }
  }
}

void clockSegR(int num) {
  digitalWrite(5, HIGH);
  segR(num);
  delay(5);
  digitalWrite(5, LOW);
}

void stepRSegR(int num) {
  for (int i = 3; i >= 0; i--) {
    seg(SEG_RESET);
    step(i);
    segR(num);
    delay(5);
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
    {0, 0, 0, 1, 1, 1, 0}, // L 11
    {1, 1, 1, 0, 1, 1, 0}, // M 12
    {0, 0, 0, 0, 1, 0, 1}, // r 13
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
  clock(1);
}
