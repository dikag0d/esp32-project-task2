#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ==== Konfigurasi Pin ====
#define LED_HIJAU 3
#define LED_PUTIH 7
#define LED_TOSCA 15
#define BUZZER 4
#define BTN1 18
#define BTN2 19
#define POT_PIN 5
#define SERVO_PIN 42
#define CLK 10
#define DT 11

// Stepper motor (tanpa driver)
#define BMINUS 37
#define BPLUS 38
#define APLUS 39
#define AMINUS 40

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Servo
Servo myservo;

// Encoder
int lastStateCLK;
int counter = 0;

// ==== Task LED Hijau ====
void TaskLED1(void *pvParameters) {
  pinMode(LED_HIJAU, OUTPUT);
  while (1) {
    digitalWrite(LED_HIJAU, !digitalRead(LED_HIJAU));
    Serial.printf("TaskLED1 running on Core %d\n", xPortGetCoreID());
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// ==== Task LED Putih ====
void TaskLED2(void *pvParameters) {
  pinMode(LED_PUTIH, OUTPUT);
  while (1) {
    digitalWrite(LED_PUTIH, !digitalRead(LED_PUTIH));
    Serial.printf("TaskLED2 running on Core %d\n", xPortGetCoreID());
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// ==== Task LED Tosca ====
void TaskLED3(void *pvParameters) {
  pinMode(LED_TOSCA, OUTPUT);
  while (1) {
    digitalWrite(LED_TOSCA, !digitalRead(LED_TOSCA));
    Serial.printf("TaskLED3 running on Core %d\n", xPortGetCoreID());
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

// ==== Task Buzzer ====
void TaskBuzzer(void *pvParameters) {
  pinMode(BUZZER, OUTPUT);
  while (1) {
    digitalWrite(BUZZER, HIGH);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    digitalWrite(BUZZER, LOW);
    vTaskDelay(1800 / portTICK_PERIOD_MS);
  }
}

// ==== Task Button 1 ====
void TaskButton1(void *pvParameters) {
  pinMode(BTN1, INPUT_PULLUP);
  while (1) {
    if (digitalRead(BTN1) == LOW) {
      Serial.printf("Button1 pressed (Core %d)\n", xPortGetCoreID());
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// ==== Task Button 2 ====
void TaskButton2(void *pvParameters) {
  pinMode(BTN2, INPUT_PULLUP);
  while (1) {
    if (digitalRead(BTN2) == LOW) {
      Serial.printf("Button2 pressed (Core %d)\n", xPortGetCoreID());
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// ==== Task Potensiometer ====
void TaskPot(void *pvParameters) {
  while (1) {
    int val = analogRead(POT_PIN);
    Serial.printf("Pot Value: %d | Core: %d\n", val, xPortGetCoreID());
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// ==== Task Servo ====
void TaskServo(void *pvParameters) {
  myservo.attach(SERVO_PIN);
  while (1) {
    for (int pos = 0; pos <= 180; pos += 10) {
      myservo.write(pos);
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    for (int pos = 180; pos >= 0; pos -= 10) {
      myservo.write(pos);
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

// ==== Task Encoder ====
void TaskEncoder(void *pvParameters) {
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  lastStateCLK = digitalRead(CLK);
  while (1) {
    int currentStateCLK = digitalRead(CLK);
    if (currentStateCLK != lastStateCLK) {
      if (digitalRead(DT) != currentStateCLK)
        counter++;
      else
        counter--;
      Serial.printf("Encoder: %d | Core: %d\n", counter, xPortGetCoreID());
    }
    lastStateCLK = currentStateCLK;
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// ==== Task OLED ====
void TaskOLED(void *pvParameters) {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  while (1) {
    display.clearDisplay();
    display.setCursor(0, 10);
    display.print("Pot:");
    display.println(analogRead(POT_PIN));
    display.setCursor(0, 30);
    display.print("Enc:");
    display.println(counter);
    display.display();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// ==== Task Stepper ====
void TaskStepper(void *pvParameters) {
  pinMode(BMINUS, OUTPUT);
  pinMode(BPLUS, OUTPUT);
  pinMode(APLUS, OUTPUT);
  pinMode(AMINUS, OUTPUT);
  int seq[4][4] = {
      {1, 0, 1, 0},
      {0, 1, 1, 0},
      {0, 1, 0, 1},
      {1, 0, 0, 1}};
  while (1) {
    for (int step = 0; step < 4; step++) {
      digitalWrite(AMINUS, seq[step][0]);
      digitalWrite(APLUS, seq[step][1]);
      digitalWrite(BPLUS, seq[step][2]);
      digitalWrite(BMINUS, seq[step][3]);
      vTaskDelay(5 / portTICK_PERIOD_MS);
    }
  }
}

// ==== Setup ====
void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9); // SDA, SCL

  // Task LED
  xTaskCreatePinnedToCore(TaskLED1, "TaskLED1", 2000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskLED2, "TaskLED2", 2000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskLED3, "TaskLED3", 2000, NULL, 1, NULL, 0);

  // Task lainnya
  xTaskCreatePinnedToCore(TaskBuzzer, "TaskBuzzer", 2000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskButton1, "TaskButton1", 2000, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskButton2, "TaskButton2", 2000, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskPot, "TaskPot", 2000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskServo, "TaskServo", 3000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskEncoder, "TaskEncoder", 2000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskOLED, "TaskOLED", 4000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskStepper, "TaskStepper", 3000, NULL, 1, NULL, 1);
}

void loop() {}
