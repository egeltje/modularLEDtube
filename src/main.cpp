#include <Arduino.h>
#include <FastLED.h>
#include "main.h"

void setup() {
  // configure serial ports
  Serial.begin(115200);
  Serial1.begin(115200);

  // configure lED strips in FastLED array
  FastLED.addLeds<CHIPSET, LED_PIN0, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN1, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN2, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN3, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN4, COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN5, COLOR_ORDER>(leds[5], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN6, COLOR_ORDER>(leds[6], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN7, COLOR_ORDER>(leds[7], NUM_LEDS).setCorrection( TypicalLEDStrip );

  delay(100); // sanity delay

  // configure switches and potentiometers
  pinMode(SWITCH_PIN0, INPUT_PULLUP);
  pinMode(SWITCH_PIN1, INPUT_PULLUP);
  pinMode(SWITCH_PIN2, INPUT_PULLUP);
}

void loop()
{
  char State = 0;
  char Param1 = ReadPot(POT_BRIGHTNESS);
  char Param2 = ReadPot(POT_EFFECT);
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random());

  switch (ReadSwitch()) {  // run simulation frame depending on state
    case 0:
      if (State != 0) {
        State = 0;
      }
      Earth(Param2);
      break;
    case 1:
      if (State != 1) {
        State = 1;
      }
      Water(Param2);
      break;
    case 2:
      if (State != 2) {
        State = 2;
      }
      Fire(Param2);
      break;
    case 3:
      if (State != 3) {
        State = 3;
      }
      Air(Param2);
      break;
    default:
      Rainbow();
  }

  FastLED.setBrightness(Param1);
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND / NUM_STRIPS);
}

void Earth(int Density) {
  int i, j;

  for (i = 0; i < NUM_STRIPS; i++) {
    for(j = 0; j < NUM_LEDS; j++) {
      leds[i][j] = 0xFF7F00;
    }
  }
}

void Water(int Waving) {
  static byte liquid[NUM_STRIPS][NUM_LEDS];
  int i, j;
//  CRGBPalette16 Palette = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);

  for (i = 0; i < NUM_STRIPS; i++) {
    // Step 1.

    // Step 2.  Droplets in each tube fall 'down' till wave level.
    for(j = 10; j < NUM_LEDS - 1; j++) {
      liquid[i][j] = liquid[i][j + 1];
      liquid[i][j + 1] = CRGB::Black;
    }
    // Step 3.  Randomly drop new 'droplets' of water at the top
    if(random8() < Waving / 8) {
      liquid[i][NUM_LEDS - 1] = CRGB::Blue;
    }
    // Step 4.  Map from heat cells to LED colors
    for(j = 0; j < NUM_LEDS; j++) {
//      CRGB color = ColorFromPalette(Palette, liquid[i][j]);
//      leds[i][j] = color;
      leds[i][j] = liquid[i][j];
    }
  }
}

void Fire(int Sparking) {
  // Inspired by Fire2012 by Mark Kriegsman, July 2012
  // as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
  // gParam1 = COOLING
  // gParam2 = SPARKING

  static byte heat[NUM_STRIPS][NUM_LEDS];
  int i, j;

  for (i = 0; i < NUM_STRIPS; i++) {
    // Step 1.  Cool down every cell a little
    for(j = 0; j < NUM_LEDS; j++) {
      heat[i][j] = qsub8(heat[i][j],  random8(0, ((FIRE_COOLING * 10) / NUM_LEDS) + 2));
    }
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for(j= NUM_LEDS - 1; j >= 2; j--) {
      heat[i][j] = (heat[i][j - 1] + heat[i][j - 2] + heat[i][j - 2] ) / 3;
    }
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if(random8() < Sparking) {
      j = random8(7);
      heat[i][j] = qadd8(heat[i][j], random8(160,255));
    }
    // Step 4.  Map from heat cells to LED colors
    for(j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor(heat[i][j]);
      leds[i][j] = color;
    }
  }
}

void Air(int Bubbling) {
  static byte gas[NUM_STRIPS][NUM_LEDS];
  int i, j;
}

void Rainbow() {
  static char Gradient;

  if (Gradient < RAINBOW_STEP) {
    Gradient = 255;
  } else {
    Gradient -= RAINBOW_STEP;
  }

  for (int i = 0; i < NUM_STRIPS; i++) {
    fill_rainbow(leds[i], NUM_LEDS, Gradient, 4);
  }
}

char ReadPot(char Channel) {
  return 63;
  // return analogRead(Channel);
}

char ReadSwitch() {
  return (!digitalRead(SWITCH_PIN0) * 1) + (!digitalRead(SWITCH_PIN1) * 2) + (!digitalRead(SWITCH_PIN2) * 4);
  //return 2;
}
