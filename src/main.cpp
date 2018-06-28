#include <Arduino.h>
#include <FastLED.h>
#include "main.h"

void setup() {
  // configure IO pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN0, INPUT);
  pinMode(BUTTON_PIN1, INPUT);
  pinMode(BUTTON_PIN2, INPUT);
  pinMode(BUTTON_PIN3, INPUT);
  pinMode(BUTTON_PIN4, INPUT);
  pinMode(BUTTON_PIN5, INPUT);
  pinMode(BUTTON_PIN6, INPUT);

  // turn on builtin LED on to show running config
  digitalWrite(LED_BUILTIN, HIGH);

  delay(100); // sanity delay

  // configure lED strips in FastLED array
  FastLED.addLeds<CHIPSET, LED_PIN0, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN1, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN2, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN3, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN4, COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN5, COLOR_ORDER>(leds[5], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN6, COLOR_ORDER>(leds[6], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN7, COLOR_ORDER>(leds[7], NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  // configure switches and potentiometers
  // TODO

  // turn off builtin LED on to show running program
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  int state = 0;
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  switch (state) {  // run simulation frame depending on state
    case 0:
      Fire();
      break;
    default:
      Light();
  }

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND / NUM_STRIPS);
}

void Light() {
  int _i, _j;

  for (_i = 0; _i < NUM_STRIPS; _i++) {
    for( int _j = 0; _j < NUM_LEDS; _j++) {
      leds[_i][_j] = 0xFF7F00;
    }
  }
}

void Fire() {
  // Fire2012 by Mark Kriegsman, July 2012
  // as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY

  static byte _heat[NUM_STRIPS][NUM_LEDS];
  int _i, _j;

  for (_i = 0; _i < NUM_STRIPS; _i++) {
    // Step 1.  Cool down every cell a little
    for(_j = 0; _j < NUM_LEDS; _j++) {
      _heat[_i][_j] = qsub8(_heat[_i][_j],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for(_j= NUM_LEDS - 1; _j >= 2; _j--) {
      _heat[_i][_j] = (_heat[_i][_j - 1] + _heat[_i][_j - 2] + _heat[_i][_j - 2] ) / 3;
    }
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if(random8() < SPARKING) {
      _j = random8(7);
      _heat[_i][_j] = qadd8(_heat[_i][_j], random8(160,255));
    }
    // Step 4.  Map from heat cells to LED colors
    for(_j = 0; _j < NUM_LEDS; _j++) {
      CRGB _color = HeatColor(_heat[_i][_j]);
      leds[_i][_j] = _color;
    }
  }
}
