/*
Game: classic SIMON
Design:
 * one led strip displays randomly one of the 4 colors every second, this nb_leds_strip times
 * the other led strip in white lights one more led for every new player_color
 Once all the colors are displayed, the first led strip displays white, the other nothing
 -> the user has to press the buttons in the correct order
 * One led strip displays the selected player_color one led by one led
 * the other displays the progression of the player (n leds = ncorrect answer)
 The player goal is to find the nb_leds_strip colors in the right order

  Warning: don't display the same player_color 2 times in a row ! (not player-friendly)
  Bug: unwanted reset happening once when uploading the code /!\ not solved
 */


/********************
        INCLUDE
*********************/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h> // used to light the led strips

/********************
    FIXED VARIABLES
*********************/

// pin # of inputs (lights and buttons)
#define LED1_PIN 12 // pin # of the first led strip
#define LED2_PIN 13 // pin # of the second led strip

#define BUTTON_R_PIN 34 // pin # of the red button
#define BUTTON_G_PIN 35 // pin # of the green button
#define BUTTON_B_PIN 25 // pin # of the blue button
#define BUTTON_Y_PIN 26 // pin # of the yellow button

// NeoPixel led strips initialisation
#define total_nb_leds_strip 10 // maximum number of leds in each strip
Adafruit_NeoPixel strip_led1(total_nb_leds_strip, LED1_PIN, NEO_GRB + NEO_KHZ800); // first led strip
Adafruit_NeoPixel strip_led2(total_nb_leds_strip, LED2_PIN, NEO_GRB + NEO_KHZ800); // second led strip

// colors RGB definition
uint32_t red = strip_led1.Color(255, 0, 0); // packed RGB of red
uint32_t green = strip_led1.Color(0,150,0); // packed RGB of green
uint32_t blue = strip_led1.Color(0, 0, 255); // packed RGB of blue
uint32_t yellow = strip_led1.Color(255, 140, 0); // packed RGB of yellow
uint32_t white = strip_led1.Color(255, 255, 255); // packed RGB of white

/********************
  GAMEPLAY VARIABLES
*********************/

int brightness = 30; // brightness of the leds (0-255)
double delay_display = 750; // waiting time between each color (in ms)
int color_before = -1; // used in the loop to have a new color every time
int player_loop_number = -1; // number of buttons pressed by the user
#define nb_leds_strip 6 // USED number of leds in each strip : DIFFICULTY (1-total_nb_leds_strip)

/********************
    OTHER VARIABLES
*********************/

// game state booleans
bool player_turn = false; // used to select stage of the game
bool player_lost = false; // turns true when a false color is chosen

int lastState_R = 1; // previous state of the red button (1: OFF, 0: PRESSED)
int lastState_G = 1; // previous state of the green button (1: OFF, 0: PRESSED)
int lastState_B = 1; // previous state of the blue button (1: OFF, 0: PRESSED)
int lastState_Y = 1; // previous state of the yellow button (1: OFF, 0: PRESSED)

int currentState_R; // current state of the red button (1: OFF, 0: PRESSED)
int currentState_G; // current state of the green button (1: OFF, 0: PRESSED)
int currentState_B; // current state of the blue button (1: OFF, 0: PRESSED)
int currentState_Y; // current state of the yellow button (1: OFF, 0: PRESSED)

// list used to display the colors and compare the results
int player_color_list[nb_leds_strip] = {0}; // list of the player color inputs
int game_color_list[nb_leds_strip] = {0}; // list of the game random color inputs

/********************
        SETUP
*********************/

void setup() {

  Serial.begin(115200); // used for text communication between the compucter and the card
  Serial.println("Reset happened"); // call serial monitor to see debug texts
  strip_led1.begin(); // Configure NeoPixel pin of the first strip for output
  strip_led2.begin(); // Configure NeoPixel pin of the second strip for output

  // initialize the pushbutton pins. INPUT_PULLUP : 0 if button pressed, else 1
  pinMode(BUTTON_R_PIN, INPUT_PULLUP);
  pinMode(BUTTON_G_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(BUTTON_Y_PIN, INPUT_PULLUP);

  // turn off all the leds of the 2 strips
  for(int i = 0; i < total_nb_leds_strip; i++){
    strip_led1.clear(); // turn of the leds of the first trip
    strip_led2.clear(); // turn of the leds of the second trip
    strip_led1.setBrightness(brightness); // set led brightness for the first strip
    strip_led2.setBrightness(brightness); // set led brightness for the second strip
    strip_led1.show(); //Transmit the first strip pixels data in RAM to NeoPixels.
    strip_led2.show(); //Transmit the first strip pixels data in RAM to NeoPixels.
  }
  
} // end of setup

/********************
        LOOP
*********************/

void loop() {
  
  ///////////////////
  // Machine turn // 
  /////////////////

  if(!player_turn){
    
    Serial.println("Machine turn");

    int rand_color = -1; // random int between 0 and 3 corresponding to the 4 possible colors
    uint32_t color_display; // random game color in packed RGB value

    // displays 3 blinks of white before beginning the sequence 
    for(int blink_w=0; blink_w < 3; blink_w++){// on 
        for(int i = 0; i < nb_leds_strip; i++){ // for every led
          strip_led1.setPixelColor(i, white);
          strip_led2.setPixelColor(i, white); 
          strip_led1.show();
          strip_led2.show();
        }
      delay(delay_display/2); // wait half the usual time
      
      for(int i = 0; i < nb_leds_strip; i++){ // off
          strip_led1.clear(); 
          strip_led2.clear();
          strip_led1.show();
          strip_led2.show();
        }
      delay(delay_display/2); // wait half the usual time
    }
    
    // displaying the sequence of nb_leds_strip colors
    for (int i = 0; i < nb_leds_strip; i++){
      
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
      for(int j = 0; j < nb_leds_strip; j++){
        strip_led1.setPixelColor(j, color_display); 
        strip_led1.show(); // refresh all the leds
      }

      // displays the progression on strip 2
      for(int j = 0; j < i + 1; j++){
        strip_led2.setPixelColor(j, white); 
        strip_led2.show(); // refresh all the leds
      }

      delay(delay_display); // wait for the wanted time between each color

    } // end of sequence display

    // when the nb_leds_strip colors are displayed, we begin the player turn
    player_turn = true;

    //turn of all the lights
    for(int j = 0; j < total_nb_leds_strip; j++){
      strip_led1.clear(); 
      strip_led2.clear(); 
      strip_led1.show();
      strip_led2.show();
    }

    Serial.println("Player turn");

  } // end of machine turn

  ///////////////////
  // Machine turn // 
  /////////////////

  else{

    // all the nb_leds_strip colors are chosen correctly : display all in green
    if(player_loop_number >= nb_leds_strip - 1){
      for(int i = 0; i < nb_leds_strip; i++){ 
        strip_led1.setPixelColor(i, green); 
        strip_led2.setPixelColor(i, green); 
        strip_led1.show();
        strip_led2.show();
      }
    }

    // the player lost in the previous loop : display all in red
    else if(player_lost){ 
      for(int i = 0; i < nb_leds_strip; i++){
        strip_led1.setPixelColor(i, red); 
        strip_led2.setPixelColor(i, red); 
        strip_led1.show();
        strip_led2.show();
      }
    }

    // the player is correct but has not finished the sequence: continue the game
    else{
      
      uint32_t player_color; // packed RGB color selected by the player
      int player_color_int = -1; // player color equivalent with [0,3] integer

      // update the current buttons states (1 if not pressed, 0 if pressed)
      currentState_R = digitalRead(BUTTON_R_PIN);
      currentState_G = digitalRead(BUTTON_G_PIN);
      currentState_B = digitalRead(BUTTON_B_PIN);
      currentState_Y = digitalRead(BUTTON_Y_PIN);

      // updates player_color and player_color_int depending on the button states
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
      
      // if any button is newly pressed (not pressed in the previous loop)
      if((lastState_R == 1 && currentState_R == 0) ||
      (lastState_G == 1 && currentState_G == 0) ||
      (lastState_B == 1 && currentState_B == 0) ||
      (lastState_Y == 1 && currentState_Y == 0)){

        player_loop_number++; // increase the player loop number

        // adds the corresponding color to the list
        player_color_list[player_loop_number] = player_color;

        // display the player sequence on the first strip
        for(int i = 0; i < nb_leds_strip; i++){ 
          strip_led1.setPixelColor(i, player_color_list[i]); 
          strip_led1.show();
        }

        Serial.println(player_color_int); // display the player selected color ([0,3] equivalent)

        // checks for player mistake
        if(player_loop_number >=0){ // don't check before the first button is pressed
          if(player_color_int != game_color_list[player_loop_number]){
            player_lost = true;
          }
        }

      } // end of button newly pressed

      // update the last button states
      lastState_R = currentState_R;
      lastState_G = currentState_G;
      lastState_B = currentState_B;
      lastState_Y = currentState_Y;
      
      delay(20); // small delay to avoid a repetition bug (two inputs instead of one)

    } // end of player input
  } // end of machine turn
} // end of loop