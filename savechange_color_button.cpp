/*
 This code lights the two led strips on white
 When a button is pressed, it lights the strips with the corresponding color,
 until a new color is pressed.
 */
#include <Arduino.h>
#include <Adafruit_NeoPixel.h> // used to light the led strips

#define LED1_PIN 12 // pin of the first led
#define LED2_PIN 13 

#define BUTTON_R_PIN 34 // pin of the red button
#define BUTTON_G_PIN 35
#define BUTTON_B_PIN 25
#define BUTTON_Y_PIN 26

int nb_leds_strip = 10; // number of leds in each strip
Adafruit_NeoPixel strip_led1(nb_leds_strip, LED1_PIN, NEO_GRB + NEO_KHZ800); // first led strip
Adafruit_NeoPixel strip_led2(nb_leds_strip, LED2_PIN, NEO_GRB + NEO_KHZ800); // second led strip

// initial color
uint32_t color = strip_led1.Color(255, 255, 255); // here white

void setup() {

  Serial.begin(115200);
  Serial.println("Reset happened");
  strip_led1.begin(); // Configure NeoPixel pin of the first strip for output
  strip_led2.begin();

  // initialize the pushbutton pins as an pull-up inputs
  pinMode(BUTTON_R_PIN, INPUT_PULLUP); //INPUT_PULLUP : 0 if button pressed, else 1 
  pinMode(BUTTON_G_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(BUTTON_Y_PIN, INPUT_PULLUP);

} 

void loop() {
  // defining the colors (would be better to do it outside the loop)
  uint32_t red = strip_led1.Color(255, 0, 0);
  uint32_t green = strip_led1.Color(0, 255, 0);
  uint32_t blue = strip_led1.Color(0, 0, 255);
  uint32_t yellow = strip_led1.Color(255, 180, 0);

  if(!digitalRead(BUTTON_R_PIN)) color = red;
  if(!digitalRead(BUTTON_G_PIN)) color = green;
  if(!digitalRead(BUTTON_B_PIN)) color = blue;
  if(!digitalRead(BUTTON_Y_PIN)) color = yellow;

  for(int i = 0; i < 10; i++){ // for every led of the strips
    strip_led1.setBrightness(200); // total brightness of the leds
    strip_led2.setBrightness(200);

    strip_led1.clear(); // Fill the whole NeoPixel strip with black.
    strip_led2.clear();

    strip_led1.setPixelColor(i, color); 
    strip_led2.setPixelColor(i, color);

    strip_led1.show(); // refresh all the leds
    strip_led2.show();
  }
  
  // debug :
  // Serial.println(digitalRead(BUTTON_R_PIN));
  // Serial.println(digitalRead(BUTTON_G_PIN));
  // Serial.println(digitalRead(BUTTON_B_PIN));
  // Serial.println(digitalRead(BUTTON_Y_PIN));

}
