//Libraries
#include <FastLED.h>//https://github.com/FastLED/FastLED
#include <math.h>

//Constants
#define NUM_STRIPS 2
#define NUM_LEDSA 173
#define NUM_LEDSB 57
#define BRIGHTNESS 10
#define LED_TYPE WS2812B
#define COLOR_ORDER RGB
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#define FRAMES_PER_SECOND 60
#define COOLING 55
#define SPARKING 120

//Parameters
const int stripPinA  = 3;
const int stripPinB  = 2;

//Variables
bool gReverseDirection = false;
int redvar = 255;
int greenvar = 0;
int bluevar = 255;
int linestep[] = {1, 1, 2, 3};

int kitty[][8] = {
  {2,7,0},
  {2,3,4,5,6,7,8},
  {2,8},
  {1,3,6,8},
  {1,9},
  {1,5,9},
  {1,8},
  {2,8},
  {3,4,5,6,7}
};

int heure[3] = {00,40,18};
int mod[6] = {60,60,24};

unsigned long startMillis; 
unsigned long currentMillis;
const unsigned long period = 1000;

//Objects
CRGB ledsA[NUM_LEDSA];
CRGB ledsB[NUM_LEDSB];

void setup() {
  //Init Serial USB
  Serial.begin(9600);
  Serial.println(F("Initialize System"));
  //Init led strips
  FastLED.addLeds<LED_TYPE, stripPinA, COLOR_ORDER>(ledsA, NUM_LEDSA);
  FastLED.addLeds<LED_TYPE, stripPinB, COLOR_ORDER>(ledsB, NUM_LEDSB);
  FastLED.setBrightness(  BRIGHTNESS );
  startMillis = millis();
}

void loop() {
  ledScenario();
}

void ledScenario(void ) { /* function ledScenario */
  for (int i = 1; i <= 15; i++) {

    timeCalc();
    redvar = 0;
    greenvar = 70;
    bluevar = 70;

    drawKitty(i, i);
    
    redvar = 255;
    greenvar = 0;
    bluevar = 55;

    drawDigi(3, 5, heure[2]/10);
    drawDigi(6, 5, heure[2]%10);
    drawDigi(11, 5, heure[1]/10);
    drawDigi(14, 5, heure[1]%10);
    
    // drawDigi(i+9, 1, 3);
    // drawDigi(i+12, 1, 4);

    FastLED.show();
    delay(100);
    fill_solid(ledsA, NUM_LEDSA, CRGB::Black);
    fill_solid(ledsB, NUM_LEDSA, CRGB::Black);
  }
}

void pixelMapper(int x, int y) {
  if (y > 17) {y = y-17;}
  if (y < 0) {y = y+17;}
  if ((y >= 5) && (y <= 11)) {
    int ry = y - 4;
    int linesize = 16 + (ry-1)%3;
    if (linesize > 17) { linesize = 17; }
    if ( x > linesize) { x = x - 17; }
    if (x > 0) {
      int i = (ry*16 + round((ry-0.5)*2/3)) - linesize + ((ry%2)*2-1) * x + ((ry+1)%2) * (linesize + 1 );
      ledsA[i-1].setRGB(greenvar, redvar, bluevar); 
    }
  } else if ((y > 11) || (y < 5)) {
    int ry;
    int stepIndex;
    int indexPadding;
    if (y > 11) {
      ry = y - 4;
      stepIndex = y - 12;
      indexPadding = 116;
    } else {
      ry = y;
      stepIndex = 4 - y;
      indexPadding = 0;
    }
    int step = linestep[ stepIndex ];
    int halfstep = round((step+0.5)/2);
    int linesize = (16 - step);
    if ( x > linesize + halfstep) { x = x - 17; }
    if ( x > halfstep) {
      int steps = 0;
      for (int i = 0; i <= stepIndex; i++) {
        steps = steps + linestep[i];
      }
      int lastpline = indexPadding + 16 * (stepIndex + 1) - steps;
      int i = lastpline - linesize + ((ry%2)*2-1) * (x - halfstep) + ((ry+1)%2) * (linesize + 1);
      
      if (y > 11) {
        ledsA[i-1].setRGB(greenvar, redvar, bluevar);
      } else {
        ledsB[i-1].setRGB(greenvar, redvar, bluevar);
      }
    }
  }
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

void drawKitty(int x, int y) {
  for (int i = 0; i <= 8; i++) {
    for (int j = 0 ; j <= 7; j++) {
      if (kitty[i][j] != 0) {
        pixelMapper(kitty[i][j]+x, i+y);
      }
    }
  }
}

void timeCalc() {
  currentMillis = millis();
  if (currentMillis - startMillis >= period) {
    startMillis = currentMillis;
    heure[0]++;//incrementation du temps
    for (int i = 0; i < 3 ; i++){
      if (heure[i] == mod[i]) { // verification du dépassement de limite
        if (i<5) {heure[i+1] ++;}
        heure[i] = 0;
      }
    }
  }
}
