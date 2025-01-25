//Libraries
#include <FastLED.h>//https://github.com/FastLED/FastLED
#include <math.h>

//Constants
#define NUM_STRIPS 1
#define NUM_LEDS 116
#define BRIGHTNESS 10
#define LED_TYPE WS2812B
#define COLOR_ORDER RGB
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#define FRAMES_PER_SECOND 60
#define COOLING 55
#define SPARKING 120

//Parameters
const int stripPin  = 3;

//Variables
bool gReverseDirection = false;
int redvar = 255;
int greenvar = 0; 
int bluevar = 255;


//Objects
CRGB leds[NUM_LEDS];

void setup() {
  //Init Serial USB
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  //Init led strips
  FastLED.addLeds<LED_TYPE, stripPin, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop() {
  ledScenario();
}

void ledScenario(void ) { /* function ledScenario */
  
  for (int i = 1; i <= 17; i++) {

    redvar = 0;
    greenvar = 70;
    bluevar = 70;

    horLine(1,i%7+1,16); 
    horLine(1,i%7+3,16); 
    vertLine(17-i, 1, 6);
    vertLine(17-i+3, 1, 6);

    greenvar = 0;
    bluevar = 255;
    redvar = 200;
   
    drawDigi(i, 1, 0);
    drawDigi(i+3, 1, 1);
    drawDigi(i+6, 1, 2);
    drawDigi(i+9, 1, 3);
    drawDigi(i+12, 1, 4);
    FastLED.show();
    delay(50);
    fill_solid(leds, NUM_LEDS, CRGB::Black);
  }
}

void pixelMapper(int x, int y) {
  int linesize = 16 + (y-1)%3;
  if (linesize > 17) { linesize = 17; }
  if ( x > 17) { x = x - 17; }
  int i = (y*16 + round((y-0.5)*2/3)) - linesize + ((y%2)*2-1) * x + ((y+1)%2) * (linesize + 1 );
  leds[i-1].setRGB(greenvar, redvar, bluevar);
}

void vertLine(int x, int y, int size) {
  for (int i = y; i <= y+size; i++ ) {
    pixelMapper(x, i);
  }
}

void horLine(int x, int y, int size) {
  for (int i = x; i <= x+size; i++ ) {
    pixelMapper(i, y);
  }
}

void drawDigi(int x, int y, int d) {
  if ((d == 0) || (d == 2) || (d == 3) || (d == 5) || (d == 6) || (d == 7) || (d == 9)) {
    pixelMapper(x, y);
  }
  if ((d == 0) || (d == 4) || (d == 5) || (d == 9)) {
    vertLine(x, y+1, 1);
  }
  if (((d >= 0) && (d <= 4)) || ((d >= 7) && (d <= 9))){
    vertLine(x+1, y+1, 1);
  }
  if (((d >= 2)&&(d <= 5)) || (d == 9)) {
    pixelMapper(x, y+3);
  }
  if ((d == 2) || (d == 0)) {
    vertLine(x, y+4, 1);
  }
  if ((d == 0) || (d == 1 ) || ((d >= 3) && (d <= 9))) {
    vertLine(x+1, y+4, 1);
  }
  if ((d == 0) || (d == 2) || (d == 3) || (d == 5) || (d == 9)) {
    pixelMapper(x, y+6);
  }
  if ((d == 6) || (d == 8)) {
    vertLine(x, y, 6);
  }
}



