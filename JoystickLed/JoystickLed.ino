// 사용 핀 정의
#define buttonA 2
#define buttonB 3
#define buttonC 4
#define buttonD 5
#define buttonE 6
#define buttonF 7
#define buttonK 8
#define LED 11

// 서보모터 정의
#include <Servo.h>

Servo myServoTop;
Servo myServoBottom;

// 사용 변수들 초기화
int xValue = 1500;
int yValue = 1500;

int analogLed = 0;

bool calibration = false;
bool quadrangle = false;
int quadrangleCount = 1000;

int before_buttonValueA = 0;
int before_buttonValueB = 0;
int before_buttonValueC = 0;
int before_buttonValueD = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // 사용 서보모터 핀 설정
  myServoBottom.attach(9);
  myServoTop.attach(10);

  // 사용 서보모터 초깃값 제어
  myServoBottom.writeMicroseconds(1500);
  myServoTop.writeMicroseconds(1500);

  // 사용 핀들에 대한 입출력을 설정합니다.
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  pinMode(buttonD, INPUT_PULLUP);
  pinMode(buttonE, INPUT_PULLUP);
  pinMode(buttonF, INPUT_PULLUP);
  pinMode(buttonK, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  // 서보모터가 사각형을 그리며 움직일 수 있도록 합니다.
  if (quadrangle == true) {
    Serial.print("quadrangleCount : ");
    Serial.println(quadrangleCount);
    if (quadrangleCount <= 2000) {
      xValueWrite(quadrangleCount);         // 1000 ~ 2000
    } else if (quadrangleCount > 2000 && quadrangleCount <= 2500) {
      yValueWrite(quadrangleCount - 750);   // 1250 ~ 1750
    } else if (quadrangleCount > 2500 && quadrangleCount <= 3500) {
      xValueWrite(4500 - quadrangleCount);  // 2000 ~ 1000
    } else if (quadrangleCount > 3500) {
      yValueWrite(5250 - quadrangleCount);  // 1750 ~ 1250
    }

    quadrangleCount+=10;

    if (quadrangleCount > 4000) quadrangleCount = 1000;
  }

  // 캘리브레이션 동작 정리
  if (calibration == true) {
    for (int x = 500; x < 2500; x+=10) {
      xValueWrite(x);
    }

    xValueWrite(1500);

    for (int y = 500; y < 2500; y+=10) {
      yValueWrite(y);
    }

    yValueWrite(1500);
    
    calibration = false;
  } 

  // 입력으로 사용하는 핀들의 값을 받아와서 변수에 저장
  int x = analogRead(0);
  int y = analogRead(1);

  int buttonValueA = digitalRead(2);
  int buttonValueB = digitalRead(3);
  int buttonValueC = digitalRead(4);
  int buttonValueD = digitalRead(5);
  int buttonValueE = digitalRead(6);
  int buttonValueF = digitalRead(7);
  int buttonValueK = digitalRead(8);

  // 사용하는 버튼을 눌렀을 때의 동작 제어
  // (이전 버튼 값과 다를 때, 즉 버튼을 꾹 눌러도 한 번만 감지)

  // A버튼을 누를 때, analogLed 변수 통해 led 세기를 0으로 제어
  if (buttonValueA == 0 && buttonValueA != before_buttonValueA) {
    analogLed = 0;
    Serial.println("Clicked A!");
    Serial.println(analogLed);
  }

  // B버튼을 누를 때, analogLed 변수 통해 led 세기를 50씩 증가
  if (buttonValueB == 0 && buttonValueB != before_buttonValueB) {
    analogLed += 50;
    if (analogLed > 250) analogLed = 250;
    Serial.println("Clicked B!");
    Serial.println(analogLed);
  }

  // C버튼을 누를 때, analogLed 변수 통해 led 세기를 250으로 제어
  if (buttonValueC == 0 && buttonValueC != before_buttonValueC) {
    analogLed = 250;
    Serial.println("Clicked C!");
    Serial.println(analogLed);
  }

  // A버튼을 누를 때, analogLed 변수 통해 led 세기를 50씩 감소
  if (buttonValueD == 0 && buttonValueD != before_buttonValueD) {
    analogLed -= 50;
    if (analogLed < 0) analogLed = 0;
    Serial.println("Clicked D!");
    Serial.println(analogLed);
  }

  // 버튼 통해 변경된 led 세기 적용
  analogWrite(LED, analogLed);

  // E버튼을 누를 때, 서보모터가 사각형을 그리는 동작을 하도록 quadrangle 변수 제어
  if (buttonValueE == 0) {
    Serial.println("Clicked E!");
    quadrangle = !quadrangle;
    Serial.println(quadrangle);
    delay(1000);
    myServoTop.writeMicroseconds(1250);
  }

  // 위 quadrangle 동작 제어 중 서보모터 관련 동작 제어 불가능하도록 막기
  if (quadrangle == false) {
    // F버튼을 누를 때, 두 서보모터 모두 랜덤 위치로 이동
    if (buttonValueF == 0) {
      Serial.println("Clicked F!");
      xValue = random(1000,2000);
      yValue = random(1000,2000);
      myServoBottom.writeMicroseconds(xValue);
      myServoTop.writeMicroseconds(yValue);
      delay(300);
    }

    // K버튼(조이스틱버튼)을 누를 때, 캘리브레이션 동작 실행
    if (buttonValueK == 0) {
      Serial.println("Clicked K!");
      calibration = true;
      Serial.println(calibration);
      delay(1000);
    }

    // 조이스틱을 x축/y축으로 움직임에 따라 서보모터를 동작
    if (x < 50) {
      for (int i = 0; i < 300; i++) {
        xValue -= 1;
        if (xValue < 500) xValue = 500;
        myServoBottom.writeMicroseconds(xValue);
        delay(2);
      }
    } else if (x > 950) {
      for (int i = 0; i < 300; i++) {
        xValue += 1;
        if (xValue > 2500) xValue = 2500;
        myServoBottom.writeMicroseconds(xValue);
        delay(2);
      }
    }

    if (y < 50) {
      for (int i = 0; i < 300; i++) {
        yValue -= 1;
        if (yValue < 500) yValue = 500;
        myServoTop.writeMicroseconds(yValue);
        delay(2);
      }
    } else if (y > 950) {
      for (int i = 0; i < 300; i++) {
        yValue += 1;
        if (yValue > 2500) yValue = 2500;
        myServoTop.writeMicroseconds(yValue);
        delay(2);
      }
    }
  }

  // loop문 마지막에 이전 값 변수에 현재 값 넣기
  before_buttonValueA = buttonValueA;
  before_buttonValueB = buttonValueB;
  before_buttonValueC = buttonValueC;
  before_buttonValueD = buttonValueD;
}

// x축 서보모터를 제어하기 쉽게하기 위한 함수
void xValueWrite(int x) {
  Serial.print("x : ");
  Serial.println(x);
  myServoBottom.writeMicroseconds(x);
  delay(2);
}

// y축 서보모터를 제어하기 쉽게하기 위한 함수
void yValueWrite(int y) {
  Serial.print("y : ");
  Serial.println(y);
  myServoTop.writeMicroseconds(y);
  delay(2);
}
