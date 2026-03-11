// 서보모터 정의
#include <Servo.h>

Servo myServoTop;
Servo myServoBottom;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // 사용 서보모터 핀 설정
  myServoBottom.attach(9);
  myServoTop.attach(10);

  // x축 끝에서 끝까지 이동 후 중앙 이동
  for (int x = 500; x < 2500; x+=10) {
      Serial.print("x : ");
      Serial.println(x);
      myServoBottom.writeMicroseconds(x);
      delay(2);
    }

    myServoBottom.writeMicroseconds(1500);

    // y축 끝에서 끝까지 이동 후 중앙 이동
    for (int y = 500; y < 2500; y+=10) {
      Serial.print("y : ");
      Serial.println(y);
      myServoTop.writeMicroseconds(y);
      delay(2);
    }

    myServoTop.writeMicroseconds(1500);
}

void loop() {
  // put your main code here, to run repeatedly:

}
