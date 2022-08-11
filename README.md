# tricky_exercice
I learn how to use the ESP32 with leds and buttons

info:
- leds not lighting up altogether: don't forget the alimentation of the card
  
To fix:
- if called in a separate function, the leds blink instead of staying on

SIMON game : 
/*
Design:
 * one led strip displays randomly one of the 4 colors every second, this 10 times
 * the other led strip in white lights the leds one by one to see the progress
 Once all the colors are displayed, all the leds shut down
 -> the user has to press the buttons in the correct order
 * One led strip displays the selected player_color one led by one led
 * the other displays nothing
 The player goal is to find the 10 colors in the right order
 if he wins, all the leds turn green, if he makes a mistake, they turn red

  Warning: don't display the same player_color 2 times in a row ! (not player-friendly)
 */

bugs: sometimes when updating the programm, the ESP32 resets in the middle of the color display
(this problem does not appear anymore when the game is reseted directly on the card)

To add: When the player loses, find a way to show him the cood color