#include <Adafruit_NeoPixel.h>


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
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define EYEPIN 12
#define PIN 8
#define NUMPIXEL 20
#define WHITE (255*65536 + 255*256 + 255)
#define RED (255*65536)
#define HALFRED (128*65536)
#define QUARTRED (64*65536)
#define EIGHTRED (32*65536)
#define GREEN (255*256)
#define EIGHTGREEN (32*256)
#define YELLOW (255*65536 + 255*256)
#define EIGHTYELLOW (32*65536 + 32*256)
#define BLUE 255
#define EIGHTBLUE 32
#define BLACK 0


////////////////////////////////////////////////
//////////  CONFIGURATION   ////////////////////
////////////////////////////////////////////////
int HEAD_SERVO_PIN_1 = 4;
int HEAD_SERVO_PIN_2 = 5;
int HEAD_SERVO_PIN_3 = 6;
int HEAD_SERVO_PIN_4 = 7;
int EYEBROWS_SERVO_PIN = 8;
int BODY_NEOPIXEL_PIN = 9;
int EYES_NEOPIXEL_PIN = 10;

//Controller button layout:
//1  2  3  4  5
//6  7  8  9  10
//11 12 13 14 15
//Controller input pins are 22-52 even numbers
// on a MEGA2560 arduino.
//pin 22 = button 1, 24 = 2, 26 = 3, etc.
int HEAD_LEFT_FAST_BUTTON = 22; //CONTROLLER BUTTON 1
int HEAD_LEFT_SLOW_BUTTON = 24; //button 2
int HEAD_JITTER_BUTTON = 26; //button 3
int HEAD_RIGHT_SLOW_BUTTON = 28; //4
int HEAD_RIGHT_FAST_BUTTON = 30; //5
int NORMAL_MODE_BUTTON = 32; //6 (happy/normal)
int ANGRY_MODE_BUTTON = 34; //7
int SAD_MODE_BUTTON = 36; //8
int SCARED_MODE_BUTTON = 38; //9
int POSSESSED_MODE_BUTTON = 40;  //10
int WING_UP_BUTTON = 42;  //11
int WING_DOWN_BUTTON = 44; //12
int FAST_FLAP_BUTTON = 46; //13
int SLOW_FLAP_BUTTON = 48; //14

//if y = controller button and x = pin number then:
//y + (y + 20) = x
//1 + (1 + 20) = 22
//2 + (2 + 20) = 24
//3 + (3 + 20) = 26
//etc.

int headDirection = 1;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXEL, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel eyes = Adafruit_NeoPixel(NUMPIXEL, EYEPIN, NEO_GRB + NEO_KHZ800);
// Larson scanner variables
long               blipColor;
long               backColor;
unsigned long      prevMillis;
long               stepTime;
int                blipIndex;
byte               blipBounce;
int                blipIncrement;

//servo stuff ////////////////
//for the wings //////////////
int FLAP_SPEED_SLOW = 10;
int FLAP_SPEED_FAST = 50;
//end servo stuff ////////////
/////////////////////////////

//stepper stuff /////////////
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 9, 10, 11, 13);

int stepCount = 0;         // number of steps the motor has taken

//constants for direction and speed
int ROTATION_LEFT = -1;
int ROTATION_RIGHT = 1;
int SPEED_FAST = 2;
int SPEED_SLOW = 10;

//end stepper ///////////////////

////////////////////////////////////////////////
//////////  END CONFIGURATION   ////////////////
////////////////////////////////////////////////

void setup() {
  initStrip();
  setEyeColor(EIGHTBLUE);
  setupInputPins();
  makeNormal(); //start in normal mode.
}

void loop() {
  // loop forever, waiting for button presses, knob turns, etc.
    unsigned long currentMillis = millis();
    
  //Larson scanner start
    executeLightStrip();
  //Larson scanner 

  myStepper.setSpeed(20);
  //testHead();
  
  //check if a button was pressed and react to it.
  doButtonAction();
}

void setupInputPins() {
	//set up the input pins as pullup so we check if they go low when pressed.
	pinMode(HEAD_LEFT_FAST_BUTTON,INPUT_PULLUP);
	pinMode(HEAD_LEFT_SLOW_BUTTON,INPUT_PULLUP);
	pinMode(HEAD_JITTER_BUTTON,INPUT_PULLUP);
	pinMode(HEAD_RIGHT_SLOW_BUTTON,INPUT_PULLUP);
	pinMode(HEAD_RIGHT_FAST_BUTTON,INPUT_PULLUP);
	pinMode(NORMAL_MODE_BUTTON,INPUT_PULLUP);
	pinMode(ANGRY_MODE_BUTTON,INPUT_PULLUP);
	pinMode(SAD_MODE_BUTTON,INPUT_PULLUP);
	pinMode(SCARED_MODE_BUTTON,INPUT_PULLUP);
	pinMode(POSSESSED_MODE_BUTTON,INPUT_PULLUP);
	pinMode(WING_UP_BUTTON,INPUT_PULLUP);
	pinMode(WING_DOWN_BUTTON,INPUT_PULLUP);
	pinMode(FAST_FLAP_BUTTON,INPUT_PULLUP);
	pinMode(SLOW_FLAP_BUTTON,INPUT_PULLUP);
}

void doButtonAction() {
	//this method checks if a button was pressed then executes the action
	//associated to that button.
	
	if(digitalRead(HEAD_LEFT_FAST_BUTTON)==LOW) {
		turnHead(ROTATION_LEFT, SPEED_FAST);
	}
	else if(digitalRead(HEAD_LEFT_SLOW_BUTTON)==LOW) {
		turnHead(ROTATION_LEFT, SPEED_SLOW);
	}
	else if(digitalRead(HEAD_JITTER_BUTTON)==LOW) {
		turnHead(ROTATION_LEFT, SPEED_FAST);
		turnHead(ROTATION_RIGHT, SPEED_FAST);
	}
	else if(digitalRead(HEAD_RIGHT_SLOW_BUTTON)==LOW) {
		turnHead(ROTATION_RIGHT, SPEED_SLOW);
	}
	else if(digitalRead(HEAD_RIGHT_FAST_BUTTON)==LOW) {
		turnHead(ROTATION_RIGHT, SPEED_FAST);
	}
	else if(digitalRead(NORMAL_MODE_BUTTON)==LOW) {
		makeNormal();
	}
	else if(digitalRead(ANGRY_MODE_BUTTON)==LOW) {
		makeAngry();
	}
	else if(digitalRead(SAD_MODE_BUTTON)==LOW) {
		makeSad();
	}
	else if(digitalRead(SCARED_MODE_BUTTON)==LOW) {
		makeScared();
	}
	else if(digitalRead(POSSESSED_MODE_BUTTON)==LOW) {
		makePossessed();
	}
	else if(digitalRead(WING_UP_BUTTON)==LOW) {
		moveWingsUp();
	}
	else if(digitalRead(WING_DOWN_BUTTON)==LOW) {
		moveWingsDown();
	}
	else if(digitalRead(FAST_FLAP_BUTTON)==LOW) {
		flapWings(FLAP_SPEED_FAST);
	}
	else if(digitalRead(SLOW_FLAP_BUTTON)==LOW) {
		flapWings(FLAP_SPEED_SLOW);
	}
	else {
		//no buttons are pressed. Stop head turn?
	}
} //end doButtonAction()

void moveWingsUp() {
	//move the wings servos all the way up.
	
}

void moveWingsDown() {
	//move the wings servos all the way down.
	
}

void flapWings(int flapSpeed) {
	//moves the wing servos up and down at the given speed.
	
}

void turnHead(int direction, int speed) {
	//turns the head in the given direction and speed.
	//direction = 1 for right or -1 for left.
	//speed = 2 is slow and 10 is fast.
	myStepper.step(direction * stepsPerRevolution / speed);
}

void testHead(){
	//test the head by moving it back and forth every second.
	if(millis()%1000 == 0){
		myStepper.step(headDirection * stepsPerRevolution/6);
		headDirection *= -1;
	}
}

void initStrip(){
    // setup the variables for the initial state/emotion
  //Larson scanner setup
  blipColor = 255*RED;
  backColor = 32*RED;
  stepTime = 75;
  blipIndex = 0;
  blipBounce = false; //true;
  blipIncrement = 1;
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  for(int i=0; i<NUMPIXEL; i++)
  {
    strip.setPixelColor(i,backColor);
  }
  strip.setPixelColor(blipIndex,blipColor);
  strip.show();
  //Larson end
}

void setEyeColor(int color){
  eyes.begin();
  for(int i=0; i<2; i++)
  {
    eyes.setPixelColor(i,color);
  }
  eyes.show();
}

void makeNormal() {
	//set all the lights, servos, etc. to make the robot look normal like when 
	//it is patroling the house with its scanners and camera.
	setEyeColor(EIGHTBLUE);
	blipColor = BLUE;
	backColor = EIGHTBLUE;
	stepTime = 75;
	blipBounce = true;
	executeLightStrip();
}

void makeHappy() {
	//set all the lights, servos, etc. to make the robot look happy.
	setEyeColor(EIGHTGREEN);
	blipColor = WHITE;
	backColor = EIGHTGREEN;
	stepTime = 150;
	executeLightStrip();
}

void makeAngry() {
	//set all the lights, servos, etc. to make the robot look angry.
	setEyeColor(RED);
	blipColor = RED;
	backColor = EIGHTRED;
	stepTime = 75;
	blipBounce = true;
	executeLightStrip();
}

void makeScared() {
  	//set all the lights, servos, etc. to make the robot look scared.
  	setEyeColor(WHITE);
	blipColor = WHITE;
	backColor = BLACK;
	stepTime = 100;
	blipBounce = true;
	executeLightStrip();
}

void makeSad() {
	//set all the lights, servos, etc. to make the robot look sad.
	setEyeColor(BLUE);
	blipColor = EIGHTBLUE;
	backColor = BLACK;
	stepTime = 10;
	blipBounce = true;
	executeLightStrip();
}

void makePossessed() {
	//set all the lights, servos, etc. to make the robot look like it is
	//possessed by a ghost.
	setEyeColor(YELLOW);
	blipColor = YELLOW;
	backColor = EIGHTYELLOW;
	stepTime = 30;
	executeLightStrip();
}

void executeLightStrip(){
  unsigned long currentMillis = millis();
  byte currentMode;
  if((prevMillis+stepTime)<=currentMillis)
  {
    prevMillis = currentMillis;
//    strip.setPixelColor(blipIndex,backColor);
      for(int i=0; i<NUMPIXEL; i++)
      {
        strip.setPixelColor(i,backColor);
      }
      
    blipIndex = blipIndex + blipIncrement;
    if(blipIndex>NUMPIXEL-1)
    {
      if(blipBounce)
      {
        blipIndex = blipIndex - 2*blipIncrement;
        blipIncrement = - blipIncrement;
      }
      else
      {
        blipIndex = 0;
      }
    }
    if(blipIndex<0)                           //Can only be negative because of a bounce
    {
     blipIndex = blipIndex - 2*blipIncrement;
     blipIncrement = -blipIncrement;                
    }    strip.setPixelColor(blipIndex,blipColor);
    strip.show();
    }
    
    if(digitalRead(2)==LOW)
    {
      blipColor = WHITE;
      backColor = EIGHTGREEN;
      stepTime = 150;
    }
    else if(digitalRead(3)==LOW)
    {
      blipColor = YELLOW;
      backColor = EIGHTYELLOW;
      stepTime = 30;
    }
    else if(digitalRead(4)==LOW)
    {
      blipColor = BLUE;
      backColor = EIGHTBLUE;
      stepTime = 10;
      blipBounce = false;
    }
    else
    {
      blipColor = RED;
      backColor = EIGHTRED;
      stepTime = 75;
      blipBounce = true;
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



