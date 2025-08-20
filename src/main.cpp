#include <Arduino.h>
#include <encoder.h>
#include <Servo.h>

#define DO_DEBUG

#ifdef DO_DEBUG
  #define st Serial.print("\t")
  #define sn Serial.print("\n")
  #define sp(x) Serial.print(x)
  #define spt(x) Serial.print(x); Serial.print("\t")
#else
  #define st ;
  #define sn ;
  #define sp(x) ;
  #define spt(x) ;
#endif


Encoder* flipper_encoder;
bool lstate;
Servo flipper;

void setup() {
  flipper_encoder = encoder_new(3);
  Serial.begin(11200);
  encoder_zero(flipper_encoder);
  lstate = true;
  flipper.attach(9);
}

void loop() {
  if (lstate) {
    digitalWrite(LED_BUILTIN, HIGH);
    lstate = false;
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    lstate = true;
  }
  // delay(20);

  encoder_update(flipper_encoder);

  if (digitalRead(2)) {
    encoder_zero(flipper_encoder);
    spt("zero");
  }

  unsigned long fctl = pulseIn(7, HIGH, 20000);
  signed short angle = flipper_encoder->angle;
  bool inrange = (angle < 325) && (angle > 5); 
  spt(inrange);
  if ((fctl < 1200 && fctl > 500) && (angle < 325)) {
    spt("Extend");
    flipper.writeMicroseconds(1500+70);
  } else if ((fctl > 1900) && (angle > 5)) {
    spt("Retract");
    flipper.writeMicroseconds(1500-60);
  } else {
    spt("Nothing");
    flipper.writeMicroseconds(1500);
  }

  // Serial.println(flipper_encoder->angle);
  spt(flipper_encoder->angle);
  spt(flipper_encoder->raw_angle);
  spt(pulseIn(7, HIGH, 20000));
  sn;
}
