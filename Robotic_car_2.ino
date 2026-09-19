#include <Wire.h>
#include <AutoCar.h>

#include <Wire.h>

#include "rgb_lcd.h"

AutoCar car;
rgb_lcd lcd;

boolean Walk_Slowly;
boolean Left_Detected;
float Left_Detected_time;
boolean Right_Detected;
float Right_Detected_Time;
boolean Forward_Detected;
float Forward_Detected_Time;
boolean Life_Detected;
float Life_Detected_Time;

void Walk_Along_Road_Slowly() {
 if (car.getRDistance() - car.getLDistance() > 43) {
  // Turn Right
  digitalWrite(7, HIGH);
  analogWrite(6, 50);
  digitalWrite(8, HIGH);
  analogWrite(9, 50);
  digitalWrite(4, LOW);
  analogWrite(5, 0);
  digitalWrite(2, LOW);
  analogWrite(10, 0);
 } else if (car.getLDistance() - car.getRDistance() > 43) {
  // Turn Left
  analogWrite(6, 0);
  digitalWrite(8, HIGH);
  analogWrite(9, 0);
  digitalWrite(4, LOW);
  analogWrite(5, 50);
  digitalWrite(2, LOW);
  analogWrite(10, 50);
 } else {
  digitalWrite(7, HIGH);
  // Go Straight
  analogWrite(6, 50);
  digitalWrite(8, HIGH);
  analogWrite(9, 50);
  digitalWrite(4, LOW);
  analogWrite(5, 50);
  digitalWrite(2, LOW);
  analogWrite(10, 50);
 }
}

void Go_Straight() {
 digitalWrite(7, HIGH);
 analogWrite(6, 100);
 digitalWrite(8, HIGH);
 analogWrite(9, 100);
 digitalWrite(4, LOW);
 analogWrite(5, 100);
 digitalWrite(2, LOW);
 analogWrite(10, 100);
}

void Turn_left() {
 digitalWrite(7, HIGH);
 analogWrite(6, 0);
 digitalWrite(8, HIGH);
 analogWrite(9, 0);
 digitalWrite(4, LOW);
 analogWrite(5, 100);
 digitalWrite(2, LOW);
 analogWrite(10, 100);
}

void Move_along_road() {
 if (car.getRDistance() - car.getLDistance() > 43) {
  Turn_right();
 } else if (car.getLDistance() - car.getRDistance() > 43) {
  Turn_left();
 } else {
  Go_Straight();
 }
}

void Turn_right() {
 digitalWrite(7, HIGH);
 analogWrite(6, 100);
 digitalWrite(8, HIGH);
 analogWrite(9, 100);
 digitalWrite(4, LOW);
 analogWrite(5, 0);
 digitalWrite(2, LOW);
 analogWrite(10, 0);
}

void Stop() {
 digitalWrite(7, HIGH);
 analogWrite(6, 0);
 digitalWrite(8, HIGH);
 analogWrite(9, 0);
 digitalWrite(4, LOW);
 analogWrite(5, 0);
 digitalWrite(2, LOW);
 analogWrite(10, 0);
}

// Try this first next time
void Temporary_storage() {
 car.probe();
 lcd.setCursor(1, 2);
 // Condition need replace with the correct one
 if (car.signDetected() && true) {
  switch (car.getSignType()) {
   case 1:
   lcd.clear();
   lcd.print("Stop");
   // Stop indefinitely?
   Stop();
    break;
   case 7:
   lcd.clear();
   // 淨係show?唔使改速度?
   lcd.print("speed 60");
   // 如果會出界，可以轉?還是必須直行?如果可以要換翻function。
   Go_Straight();
   delay(2 * 1000);
   lcd.clear();
   Stop();
   delay(1 * 1000);
    break;
   case 8:
   lcd.clear();
   lcd.print("Left");
   Turn_left();
   delay(2 * 1000);
   Stop();
   lcd.clear();
   delay(1 * 1000);
    break;
   case 9:
   lcd.clear();
   lcd.print("Right");
   Turn_right();
   delay(2 * 1000);
   Stop();
   lcd.clear();
   delay(1 * 1000);
    break;
   case 15:
   lcd.clear();
   lcd.print("speed 50");
   // 係慢行直至下一個標誌被檢測?還是係慢行3秒就可以?
   Walk_Along_Road_Slowly();
   delay(3 * 1000);
   Stop();
   lcd.clear();
   delay(1 * 1000);
    break;
  }
 }
}

void Main_Programme() {
 car.probe();
 if (car.getRDistance() > 0 || car.getLDistance() > 0) {
  Move_along_road();
 }
}

void setup()
{
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(10, OUTPUT);
  Wire.begin();
  Serial.begin(9600);
  lcd.begin(16, 2);
  Walk_Slowly = false;
  Left_Detected = false;
  Left_Detected_time = 0;
  Right_Detected = false;
  Right_Detected_Time = 0;
  Forward_Detected = false;
  Forward_Detected_Time = 0;
  Life_Detected = false;
  Life_Detected_Time = 0;
  car.init();
 car.initMLPModule();
 car.initEdgeDetectModule();
 car.initHoughModule();
 car.configDriverless(1);
 lcd.clear();
}


void loop()
{
  car.probe();
  lcd.setCursor(1, 2);
  // If the left is detected and it's already bene
  if (millis() - Left_Detected_time >= 2000 && Left_Detected) {
   Left_Detected = false;
  }
  // If the left is detected and it's already bene
  if (millis() - Right_Detected_Time >= 2000 && Right_Detected) {
   Right_Detected = false;
  }
  // If the left is detected and it's already bene
  if (millis() - Forward_Detected_Time >= 2000 && Forward_Detected) {
   Forward_Detected = false;
   lcd.clear();
  }
  // If the left is detected and it's already bene
  if (millis() - Life_Detected_Time >= 3000 && Life_Detected) {
   Left_Detected = false;
  }
  // Condition need replace with the correct one
  if (car.signDetected() && true) {
   switch (car.getSignType()) {
    case 1:
    lcd.print("Stop");
    Stop();
    while (true);
     break;
    case 7:
    if (!Forward_Detected) {
     Forward_Detected = true;
     Forward_Detected_Time = millis();
     Go_Straight();
     lcd.clear();
     // 淨係show?唔使改速度?
     lcd.print("speed 60");
     delay(2 * 1000);
     lcd.clear();
     Stop();
    }
     break;
    case 8:
    if (!Left_Detected) {
     Left_Detected = true;
     Left_Detected_time = millis();
     lcd.clear();
     lcd.print("Left");
     Turn_left();
     delay(2 * 1000);
     Stop();
     lcd.clear();
     delay(1 * 1000);
    }
     break;
    case 9:
    if (!Right_Detected) {
     Right_Detected = true;
     Right_Detected_Time = millis();
     lcd.clear();
     lcd.print("Right");
     Turn_right();
     delay(2 * 1000);
     Stop();
     lcd.clear();
     delay(1 * 1000);
    }
     break;
    case 15:
    if (!Life_Detected) {
     Life_Detected = true;
     Life_Detected_Time = millis();
     lcd.clear();
     lcd.print("speed 50");
     // 係慢行直至下一個標誌被檢測?還是係慢行3秒就可以?
     Walk_Along_Road_Slowly();
     delay(3 * 1000);
     Stop();
     lcd.clear();
     delay(1 * 1000);
    }
     break;
   }
  }

}