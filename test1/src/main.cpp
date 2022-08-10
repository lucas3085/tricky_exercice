/*
Game: classic SIMON
Design:
 * one led strip displays randomly one of the 4 colors every second, this 10 times
 * the other led strip in white lights one more led for every new player_color
 Once all the colors are displayed, the first led strip displays white, the other nothing
 -> the user has to press the buttons in the correct order
 * One led strip displays the selected player_color one led by one led
 * the other displays the progression of the player (n leds = ncorrect answer)
 The player goal is to find the 10 colors in the right order

  Warning: don't display the same player_color 2 times in a row ! (not player-friendly)
 */


/********************
        INCLUDE
*********************/
#include <Arduino.h>
#include <Adafruit_NeoPixel.h> // used to light the led strips


/********************
    FIXED VARIABLES
*********************/
#define LED1_PIN 12 // # pin of the first led
#define LED2_PIN 13 

#define nb_leds_strip 10 // number of leds in each strip

#define BUTTON_R_PIN 34 // # pin of the red button
#define BUTTON_G_PIN 35
#define BUTTON_B_PIN 25
#define BUTTON_Y_PIN 26

Adafruit_NeoPixel strip_led1(nb_leds_strip, LED1_PIN, NEO_GRB + NEO_KHZ800); // first led strip
Adafruit_NeoPixel strip_led2(nb_leds_strip, LED2_PIN, NEO_GRB + NEO_KHZ800); // second led strip


bool player_turn = false; // used to select stage of the game
bool player_lost = false; // turns true when a false color is chosen

int brightness = 100; // brightness of the leds (0-255)
double delay_display = 500; // in ms

// defining the colors 
uint32_t red = strip_led1.Color(255, 0, 0);
uint32_t green = strip_led1.Color(0,150,0);
uint32_t blue = strip_led1.Color(0, 0, 255);
uint32_t yellow = strip_led1.Color(255, 140, 0);
uint32_t white = strip_led1.Color(255, 255, 255);

int color_before = -1; // used in the loop to have a new color every time
int player_loop_number = -1; // number of buttons pressed by the user

int lastState_R = 0; // the previous state from the input pin
int lastState_G = 0;
int lastState_B = 0;
int lastState_Y = 0;

int currentState_R; // the current reading from the input pin
int currentState_G;
int currentState_B;
int currentState_Y;

int player_color_list[10] = {0};
int game_color_list[10] = {0};

/********************
        SETUP
*********************/
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

/********************
        LOOP
*********************/

void loop() {
  // Machine turn
  int rand_color;
  uint32_t color_display;

  if(!player_turn){ // first the program displays the colors 1 by 1

    // displays white before beginning the sequence 
    for(int i = 0; i < 10; i++){ // for every led of the first strip
        strip_led1.setBrightness(brightness); // total brightness of the leds
        strip_led1.setPixelColor(i, white); 
        strip_led1.show(); // refresh all the leds
    }
    delay(delay_display);

    // displaying the sequence of 10 colors
    for (int i = 0; i < 10; i++){ // displays 10 colors, 1 color/second

      // selection of a new random color
      do{
        color_before = rand_color;
        rand_color = random(4);
      }
      while(color_before == rand_color); // force a different color than the previous one

      if(rand_color == 0) color_display = red;
      if(rand_color == 1) color_display = green;
      if(rand_color == 2) color_display = blue;
      if(rand_color == 3) color_display = yellow;

      game_color_list[i] = rand_color;
      // prints out solution for debug:
      Serial.println(rand_color);

      // displays the color on strip 1
      for(int j = 0; j < 10; j++){ // for every led of the first strip
        strip_led1.setBrightness(brightness); // total brightness of the leds
        strip_led1.setPixelColor(j, color_display); 
        strip_led1.show(); // refresh all the leds
      }

      // displays the progression on strip 2
      for(int j = 0; j < i + 1; j++){ // for every led of the first strip
        strip_led2.setBrightness(brightness); // total brightness of the leds
        strip_led2.setPixelColor(j, white); 
        strip_led2.show(); // refresh all the leds
      }

      delay(delay_display);
    }
    player_turn = true; // the lights were displayed, now player turn

    for(int i = 0; i < 10; i++){ // for every led of the strips
      strip_led1.clear(); 
      strip_led2.clear(); 

      strip_led1.show(); // refresh all the leds
      strip_led2.show(); // refresh all the leds
      }
  }

  else{
    // Player turn 

    if(player_loop_number >= 9){ // the 10 colors are chosen correctly : display all in green
      for(int i = 0; i < 10; i++){ 
        strip_led1.setPixelColor(i, green); 
        strip_led2.setPixelColor(i, green); 
        strip_led1.show(); // refresh all the leds
        strip_led2.show(); // refresh all the leds
      }
    }

    else if(player_lost){ // lost in the previous loop : display all in red
      for(int i = 0; i < 10; i++){
        strip_led1.setPixelColor(i, red); 
        strip_led2.setPixelColor(i, red); 
        strip_led1.show(); // refresh all the leds
        strip_led2.show(); // refresh all the leds
      }
    }

    else{ // the game still runs
      
      
      
      uint32_t player_color;
      int player_color_int = -1;

      // buttons states (1 if not pressed, 1 if pressed)
      currentState_R = digitalRead(BUTTON_R_PIN);
      currentState_G = digitalRead(BUTTON_G_PIN);
      currentState_B = digitalRead(BUTTON_B_PIN);
      currentState_Y = digitalRead(BUTTON_Y_PIN);

      if(!currentState_R){
        player_color = red;
        player_color_int = 0;}
      if(!currentState_G){
        player_color = green;
        player_color_int = 1;}
      if(!currentState_B){
        player_color = blue;
        player_color_int = 2;}
      if(!currentState_Y){
        player_color = yellow;
        player_color_int = 3;}
      
      // detects if any button is pushed then released
      if((lastState_R == 1 && currentState_R == 0) ||
      (lastState_G == 1 && currentState_G == 0) ||
      (lastState_B == 1 && currentState_B == 0) ||
      (lastState_Y == 1 && currentState_Y == 0)){

        player_loop_number++;

        // adds the corresponding color to the list
        player_color_list[player_loop_number] = player_color;

        for(int i = 0; i < 10; i++){ // for every led of the first strip
          strip_led1.setPixelColor(i, player_color_list[i]); 
          strip_led1.show(); // refresh all the leds
        }

        // checks for mistake
        if(player_loop_number >=0){ // don't check before the first button is pressed
          if(player_color_int != game_color_list[player_loop_number]){
            player_lost = true;
          }
        }
      }
      lastState_R = currentState_R;
      lastState_G = currentState_G;
      lastState_B = currentState_B;
      lastState_Y = currentState_Y;
      

      

    }
    
  
  }
  
}