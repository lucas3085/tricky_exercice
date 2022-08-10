#include <Arduino.h>
#include <Adafruit_NeoPixel.h> // used to light the led strips

int led1_pin = 12;
int led2_pin = 13;

Adafruit_NeoPixel strip_led1(10, led1_pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_led2(10, led2_pin, NEO_GRB + NEO_KHZ800);

void setup() {

  strip_led1.begin();
  strip_led2.begin();
  Serial.begin(115200);
  Serial.println("Reset happened");

}

void loop() {

// light every led of the the led strips
  for(int i = 0; i < 10; i++){
    strip_led1.setBrightness(200);
    strip_led2.setBrightness(200);

    strip_led1.clear();
    strip_led2.clear();

    strip_led1.setPixelColor(i, 255, 190, 0);
    strip_led2.setPixelColor(i, 255, 190, 0);

    strip_led1.show();
    strip_led2.show();
  }
  
}