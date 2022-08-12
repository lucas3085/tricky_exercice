#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

int DataIn1 = 12;
int DataIn2 = 13;

Adafruit_NeoPixel pixels1(10, DataIn1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2(10, DataIn2, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels1.begin();
  pixels2.begin();
  Serial.begin(115200);


}

void loop() {
  for(int i = 0; i < 10; i++){
    pixels1.setBrightness(255);
  pixels1.clear();
  pixels1.setPixelColor(i, 255, 190, 0);
  pixels1.show();
  }
  for(int i = 0; i < 10; i++){
    pixels2.setBrightness(255);
  pixels2.clear();
  pixels2.setPixelColor(i, 0, 0, 255);
  pixels2.show();
  } 
  
}