/*
 Basic code to detect a button
*/
#include <Arduino.h>

#define BUTTON_R_PIN 34 // pin of the red button

void setup() {

  Serial.begin(115200);
  Serial.println("Reset happened");
  pinMode(BUTTON_R_PIN, INPUT_PULLUP); //INPUT_PULLUP : 0 if button pressed, else 1 
  Serial.println("pinMode set.");

} 

void loop() {
  Serial.println(digitalRead(BUTTON_R_PIN));
  delay(500);
}