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

  myServoBottom.writeMicroseconds(1500);
  myServoTop.writeMicroseconds(1500);
}

void loop() {
  // put your main code here, to run repeatedly:

}
