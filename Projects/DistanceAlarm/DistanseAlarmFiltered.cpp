#include <Arduino.h>
// Код с прошлого файла, но + фильтрация через стек и мидиану
const int red_pin = 2;
const int yellow_pin = 3;
const int green_pin = 4;

const int echo_pin = 8;
const int trig_pin = 7;

const int signal_pin = 12;

const int red_dist = 7;
const int yellow_dist = 14;

const int STACK_SIZE = 7; // размер стека
float stack[STACK_SIZE] = {0.0}; // создание заполение нулями

int add_counter = 0;  // НЕ КОНСТАНТА, счетчик при добавлении в стек

float get_dist(int, int);
float calculateMedian(float*, int);
void bubbleSort(float*, int);

bool signal = false;

void setup() {
  Serial.begin(115200); 
  pinMode(red_pin, OUTPUT);
  pinMode(yellow_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);

  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

void loop() {
  float dist = get_dist(trig_pin, echo_pin);

  if (dist < -1){
    dist = 0.0;
  }
  if (dist > 100){
    dist = 0.0;
  }

  Serial.print(">dist:");
  Serial.println(dist);
  // Убираем выше 100 и ниже нуля, такие значения наш датчик не умеет обрабатывать, это заведомо шум!
  if (dist != 0.0){ // тут мы есил не ноль, то добавим в наш стек, если вы потом будете фильтрованым значением упралять светодиодами, то лучше убрать отсечения нуля, но иногда будут легкие помехи
    stack[add_counter] = dist;
    add_counter++;} // добавили (строка выше), повысили счетчик
  if (add_counter == STACK_SIZE){
    add_counter = 0; // заполнили стек! счетчик обнулим
  }
  float filtered_dist = calculateMedian(stack, STACK_SIZE);  // нашли медиану

  Serial.print(">filtred_dist:");
  Serial.println(filtered_dist);

  // Светодиоды тут все так же управляются не фильтрованной длинной, убираем 49 строке if и тогда можем его использовать
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
  if (signal){
    tone(signal_pin, 1000);
  }
  else{
    noTone(signal_pin);
  }
  delay(100);
}


float get_dist(int trig, int echo) {
  digitalWrite(trig, 1);
  delayMicroseconds(10);
  digitalWrite(trig, 0);
  int us = pulseIn(echo, HIGH);
  return us / 58.0;
}

// Сортировка пузырьком 
void bubbleSort(float* array, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                float temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

// Находим медиану наших значений
float calculateMedian(float* inputStack, int size) {
    float sortedArray[size];
    
    for (int i = 0; i < size; i++) {
        sortedArray[i] = inputStack[i];
    }

    bubbleSort(sortedArray, size);
    return sortedArray[size / 2];
}
