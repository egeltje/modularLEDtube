#include <Arduino.h>
#include <FastLED.h>
#include "main.h"

void setup() {

  // configure IO pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DIGITAL_PIN0, INPUT);
  pinMode(DIGITAL_PIN1, INPUT);
  pinMode(DIGITAL_PIN2, INPUT);

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

  // configure switches and potentiometers
  // TODO

  // turn off builtin LED on to show running program
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  char state = ReadDigital();
  char Param1 = ReadAnalog(AD_CHANNEL0_PIN);
  char Param2 = ReadAnalog(AD_CHANNEL1_PIN);
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  switch (state) {  // run simulation frame depending on state
    case 0:
      Earth(Param2);
      break;
    case 1:
      Water(Param2);
      break;
    case 2:
      Fire(Param2);
      break;
    case 3:
      Air(Param2);
      break;
    default:
      Earth(Param2);
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
}

char ReadAnalog(int Channel) {
  return 63;
}

char ReadDigital() {
  return 2;
}
