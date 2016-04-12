#include <Adafruit_GFX.h>
#include <gfxfont.h>

/////////////////////////////////////////////////////////////////////
//BATS prop robot emotion engine.
//For the 315 Wicked Way movie.
//http://www.315wickedway.com/
//This sketch will take simple switch button inputs to set the robot's "emotion" like:
//"normal/scanning", "scared/fear", "angry", "confused", "sad", "surprised", "disgusted", 
//"dying" and "possessed by a ghost".
//There will also be a knob or left/right buttons to rotate the robot's head.
//Melbourne Makerspace  http://melbournemakerspace.org/
//Josh Pritt  ramgarden@gmail.com
//April 11, 2016
//version 1.0
//////////////////////////////////////////////////////////

////////////////////////////////////////////////
//////////  CONFIGURATION   ////////////////////
int HEAD_SERVO_PIN_1 = 4;
int HEAD_SERVO_PIN_2 = 5;
int HEAD_SERVO_PIN_3 = 6;
int HEAD_SERVO_PIN_4 = 7;
int EYEBROWS_SERVO_PIN = 8;
int BODY_NEOPIXEL_PIN = 9;
int LEFT_EYE_NEOPIXEL_PIN = 10;
int RIGHT_EYE_NEOPIXEL_PIN = 11;

//////////  END CONFIGURATION   ////////////////

void setup() {
  // setup the variables for the initial state/emotion

}

void loop() {
  // loop forever, waiting for button presses, knob turns, etc.
  
}

void makeNormal() {
  //set all the lights, servos, etc. to make the robot look normal like when it is
  //patroling the house with its scanners and camera.
  
}

void makeHappy() {
  //set all the lights, servos, etc. to make the robot look happy.
}

void makeAngry() {
  //set all the lights, servos, etc. to make the robot look angry.
}

void makeScared() {
  //set all the lights, servos, etc. to make the robot look scared.
}

void makeSad() {
  //set all the lights, servos, etc. to make the robot look sad.
}

void makePossessed() {
  //set all the lights, servos, etc. to make the robot look like it is
  //possessed by a ghost.
}






