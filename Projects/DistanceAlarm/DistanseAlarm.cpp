#include <Arduino.h>  // в arduino ide можно это убрать, а так же можно не объявлять фукнцию, а сразу ее прописывать! 
// Код писался в VS code при момощи Platformio, чего и вам советую научиться делать.
/* platformio.ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
monitor_speed = 115200  <-- важно для Serial Porter или другой визуализации
*/

const int red_pin = 2;
const int yellow_pin = 3;
const int green_pin = 4;

const int echo_pin = 8;
const int trig_pin = 7;

const int signal_pin = 12;

const int red_dist = 7;
const int yellow_dist = 14;

float get_dist(int, int);  // объясвялем функцию, в arduino ide тут ее можно просто написать

bool signal = false;

void setup() {
  Serial.begin(115200); 
  pinMode(red_pin, OUTPUT);
  pinMode(yellow_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);

  pinMode(trig_pin, OUTPUT); // trig выход дальнометра
  pinMode(echo_pin, INPUT);  // echo вход дальнометра
}

void loop() {
  float dist = get_dist(trig_pin, echo_pin);

  if (dist < -1){
    dist = 0.0;
  }
  if (dist > 100){
    dist = 0.0;
  }

  // Вывод >name для графика в Serial Porter, вывод нужен только для debug
  Serial.print(">dist:");
  Serial.println(dist);

  // логика светодиодов
  if (dist > 0 && dist <= red_dist) {
    digitalWrite(red_pin, 1);
    digitalWrite(yellow_pin, 0);
    digitalWrite(green_pin, 0);
    signal = true;
  }
  else if (dist > red_dist && dist <= yellow_dist) {
    digitalWrite(red_pin, 0);
    digitalWrite(yellow_pin, 1);
    digitalWrite(green_pin, 0);
    signal = false;
  }
  else if (dist > yellow_dist) {
    digitalWrite(red_pin, 0);
    digitalWrite(yellow_pin, 0);
    digitalWrite(green_pin, 1);
    signal = false;
  }
  else {
    digitalWrite(red_pin, 0);
    digitalWrite(yellow_pin, 0);
    digitalWrite(green_pin, 0);
    signal = false;
  }
  // Работа сигналки
  if (signal){
    tone(signal_pin, 1000);
  }
  else{
    noTone(signal_pin);
  }
  delay(100);
}

// находим дистанцию по времени на которое объект отразид волну, которую датчик ему послал
float get_dist(int trig, int echo) {
  digitalWrite(trig, 1);
  delayMicroseconds(10);
  digitalWrite(trig, 0);
  int us = pulseIn(echo, HIGH);
  return us / 58.0;  // деление учитываю формулу преобразования (есть в интернете)
}
