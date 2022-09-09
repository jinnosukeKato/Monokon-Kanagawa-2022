#define SEG_RESET 10
#define SEG_L 11
#define SEG_M 12
#define SEG_R 13
#define SEG_D 14
#define SEG_B 15
#define SEG_S 16

#define SW_PIN 22
#define SW_ON HIGH
#define VR_PIN A2
#define PHOTO_PIN 24
#define PHOTO_COVERED LOW


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

int leftDisp = SEG_D;
int rightDisp;
void loop() {
  // 表示
  segW(leftDisp, rightDisp);

  // 可変抵抗
  if (analogRead(VR_PIN) < 400) {
    // 左
    rightDisp = SEG_L;
  } else if (analogRead(VR_PIN) > 600) {
    // 右
    rightDisp = SEG_R;
  } else {
    // 中央付近
    rightDisp = SEG_M;
  }

  // ボタン押下
  while (digitalRead(SW_PIN) == SW_ON) {
    segW(leftDisp, rightDisp);
    if (digitalRead(SW_PIN) != SW_ON) {
      switch (leftDisp) {
        case SEG_D:
          leftDisp = SEG_B;
          break;
        case SEG_B:
          leftDisp = SEG_S;
          break;
        case SEG_S:
          leftDisp = SEG_D;
          break;
      }
    }
  }

  // 受光状態の処理
  while (digitalRead(PHOTO_PIN) != PHOTO_COVERED) {
    static boolean isMotorRotating = false;
    static boolean motorDir = false;

    switch (leftDisp) {
      case SEG_D:
        // DCモータ
        if (!isMotorRotating) {
          isMotorRotating = true;
          switch (rightDisp) {
            // 左回転
            case SEG_L:
              digitalWrite(16, LOW);
              digitalWrite(17, HIGH);
              clockSegW(leftDisp, rightDisp);
              break;
            case SEG_R:
              // 右回転
              digitalWrite(16, HIGH);
              digitalWrite(17, LOW);
              clockSegW(leftDisp, rightDisp);
              break;
            case SEG_M:
              // 停止
              isMotorRotating = false;
              break;
          }
        }
        segW(leftDisp, rightDisp);
        break;

      case SEG_B:
        // ブザー
        segW(leftDisp, rightDisp);
        analogWrite(4, 100);
        break;

      // ステッピング
      case SEG_S:
        switch (rightDisp) {
          // 左回転
          case SEG_L:
            segWStepL(leftDisp, rightDisp);
            break;
          case SEG_R:
            // 右回転
            segWStepR(leftDisp, rightDisp);
            break;
          case SEG_M:
            // 停止
            segW(leftDisp, rightDisp);
            break;
        }
        break;
    }

    // 離脱処理
    if (digitalRead(PHOTO_PIN) == PHOTO_COVERED) {
      switch (leftDisp) {
        case SEG_D:
          if (isMotorRotating) {
            isMotorRotating = !isMotorRotating;
            digitalWrite(16, HIGH);
            digitalWrite(17, HIGH);
            clockSegW(leftDisp, rightDisp);
          }
          break;
        case SEG_B:
          analogWrite(4, 0);
          break;
        case SEG_S:
          segW(leftDisp, rightDisp);
          break;
      }
    }
  }
}

void clockSegW(int l, int r) {
  digitalWrite(5, HIGH);
  segW(l, r, 8);
  digitalWrite(5, LOW);
}

void segWStepR(int l, int r) {
  for (int i = 3; i >= 0; i--) {
    seg(SEG_RESET);
    step(i, 1);
    segR(r);
    delay(5);
    segL(l);
    delay(5);
  }
}

void segWStepL(int l, int r) {
  for (int i = 0; i < 4; i++) {
    seg(SEG_RESET);
    step(i, 1);
    segR(r);
    delay(5);
    segL(l);
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
    {0, 1, 1, 1, 1, 0, 1}, // d 14
    {0, 0, 1, 1, 1, 1, 1}, // b 15
    {0, 0, 1, 1, 0, 1, 1}, // s 16
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

void segW(int l, int r, int duration) {
  const int DISP_MS = 4;
  for (int i = 0; i <= duration; i += DISP_MS) {
    segL(l);
    delay(DISP_MS / 2);
    segR(r);
    delay(DISP_MS / 2);
  }
}

void segW(int l, int r) {
  segW(l, r, 0);
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
  step(n, 5);
}

void step(int n, int clockMs) {
  const int PTN[4][4] = {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 0, 0, 1}
  };

  for (int i = 12; i <= 15; i++) {
    digitalWrite(i, PTN[n][i - 12]);
  }
  clock(clockMs);
}

void stepL(int angle) {
  for (int i = angle; i >= 0; i -= 3) {
    step(i % 4);
  }
}

void stepL() {
  for (int i = 0; i < 4; i++) {
    step(i);
  }
}

void stepR(int angle) {
  for (int i = 0; i <= angle; i += 3) {
    step(i % 4);
  }
}

void stepR() {
  for (int i = 3; i >= 0; i--) {
    step(i);
  }
}
