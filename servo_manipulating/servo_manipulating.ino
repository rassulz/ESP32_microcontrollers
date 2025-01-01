#include <ESP32Servo.h>

Servo myServo;  // Создаем объект для сервопривода
int angle = 0;  // Переменная для хранения текущего угла

void setup() {
  Serial.begin(115200);     // Начинаем серийную связь
  myServo.attach(18);       // Подключаем серво к GPIO 18
  myServo.write(angle);     // Устанавливаем начальный угол
  Serial.println("Введите угол между 0 и 180:");
}

void loop() {
  // Проверяем, доступен ли ввод данных40
  
  if (Serial.available() > 0) {
    int input = Serial.parseInt();  // Считываем целочисленный ввод

    // Проверяем, входит ли введенное значение в диапазон 0-180
    if (input >= 0 && input <= 180) {
      angle = input;                // Обновляем переменную угла
      myServo.write(angle);         // Устанавливаем серво на новый угол
      Serial.print("Сервопривод установлен на угол: ");
      Serial.println(angle);
    } else {
      Serial.println("Некорректный угол! Введите значение от 0 до 180.");
    }

    // Очищаем буфер после обработки ввода
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}
