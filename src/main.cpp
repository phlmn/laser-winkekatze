#include <Arduino.h>
#include <Servo.h>
#include <DmxInput.h>

#define PIN_SERVO_ARM 0
#define PIN_SERVO_BODY 1
#define PIN_SERVO_EYE_L 4
#define PIN_SERVO_EYE_R 5
#define PIN_SERVO_HEAD 6

#define PIN_LASER_L 2
#define PIN_LASER_R 3

#define PIN_DMX 7
#define DMX_START_CHANNEL 1

enum class DmxChannel {
  SERVO_ARM,
  SERVO_BODY,
  SERVO_HEAD,

  SERVO_EYE_L,
  SERVO_EYE_R,
  LASER_EYE_L,
  LASER_EYE_R,

  COUNT,
};

Servo servo_arm;
Servo servo_body;
Servo servo_head;
Servo servo_eye_l;
Servo servo_eye_r;

DmxInput dmx_input;

int value_from_dmx(int raw, int min, int max, bool invert = false) {
  int range = max - min;
  if (invert) raw = 255 - raw;

  return round((raw / 255.0 * range) + min);
}

uint8_t read_dmx(uint8_t* buffer, DmxChannel channel) {
  return buffer[(uint)channel + DMX_START_CHANNEL];
}

uint8_t buffer[512];

void __isr dmxDataRecevied(DmxInput* instance) {
  if (buffer[0] != 0) return;

  servo_head.write(value_from_dmx(read_dmx(buffer, DmxChannel::SERVO_HEAD), 10, 170));
  servo_eye_l.write(value_from_dmx(read_dmx(buffer, DmxChannel::SERVO_EYE_L), 34, 64));
  servo_eye_r.write(value_from_dmx(read_dmx(buffer, DmxChannel::SERVO_EYE_R), 90, 120, true));

  servo_body.write(value_from_dmx(read_dmx(buffer, DmxChannel::SERVO_ARM), 0, 180));
  servo_arm.write(value_from_dmx(read_dmx(buffer, DmxChannel::SERVO_BODY), 40, 150));

  digitalWrite(PIN_LASER_L, value_from_dmx(read_dmx(buffer, DmxChannel::LASER_EYE_L), 0, 1));
  digitalWrite(PIN_LASER_R, value_from_dmx(read_dmx(buffer, DmxChannel::LASER_EYE_R), 0, 1));
}

void setup() {
  pinMode(PIN_LASER_L, OUTPUT);
  pinMode(PIN_LASER_R, OUTPUT);

  servo_eye_l.attach(PIN_SERVO_EYE_L); // 25 - 70
  servo_eye_r.attach(PIN_SERVO_EYE_R); // 70 - 120
  servo_head.attach(PIN_SERVO_HEAD); // 10 - 170
  servo_arm.attach(PIN_SERVO_ARM); // 10 - 170
  servo_body.attach(PIN_SERVO_BODY); // 10 - 170

  dmx_input.begin(PIN_DMX, 0, (uint)DmxChannel::COUNT);

  dmx_input.read_async(buffer, dmxDataRecevied);
}

void loop() {
  // noop
}
