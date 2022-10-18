#include <Arduino.h>
#include <Servo.h>

#define PIN_LASER_L 2
#define PIN_LASER_R 3
#define PIN_SERVO_EYE_L 4
#define PIN_SERVO_EYE_R 5
#define PIN_SERVO_HEAD 6

Servo servo_head;
Servo servo_eye_l;
Servo servo_eye_r;

void setup() {
  pinMode(PIN_LASER_L, OUTPUT);
  pinMode(PIN_LASER_R, OUTPUT);

  servo_eye_l.attach(PIN_SERVO_EYE_L); // 25 - 70
  servo_eye_r.attach(PIN_SERVO_EYE_R); // 70 - 120
  servo_head.attach(PIN_SERVO_HEAD); // 10 - 170

  digitalWrite(PIN_LASER_L, HIGH);
  digitalWrite(PIN_LASER_R, HIGH);
}

int tick;

void loop() {

  float sine = sin(tick/10000.0);
  servo_head.write(sine * 80 + 90);
  servo_eye_l.write(sine * 20 + 47); 
  servo_eye_r.write(-sine * 20 + 95); 

  tick++;
}
