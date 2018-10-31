#include <Arduino.h>
#include <FastLED.h>
#include <stdint.h>
#include "main.h"

void setup() {
  // configure serial ports
  // Serial.begin(115200);
  // Serial1.begin(9600);

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

  // Initialize Perlin coordinates to some random values
  x = random16();
  y = random16();
  z = random16();
}

void loop()
{
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random());

  EVERY_N_MILLISECONDS(FRAME_DELAY) {
    // read potmeters and determine state
    Param1 = ReadPot(POT_EFFECT);
    Param2 = ReadPot(POT_EFFECT_MOD1);
    Param3 = ReadPot(POT_EFFECT_MOD2);
    Param4 = ReadPot(POT_EFFECT_MOD3);
    State = ReadState(Param1);

    // run simulation frame depending on state
    switch (State) {
      case 2:
        Light();
        break;
      case 3:
        Earth(Param3, Param4);
        break;
      case 4:
        Water(Param3, Param4);
        break;
      case 5:
        Fire(Param3, Param4);
        break;
      case 6:
        Air(Param3, Param4);
        break;
      default:
        Rainbow();
    }

    // set brigtness and display the calculated frame
    FastLED.setBrightness(Param2);
    FastLED.show();
  }
}

void Light() {
  // set all LEDs to full white
  // no params, only overall brightness
  uint8_t i, j;

  for (i = 0; i < NUM_STRIPS; i++) {
    for (j = 0; j < NUM_LEDS; j++) {
      leds[i][j] = 0xffffff;
    }
  }
}

void Earth(uint8_t Density, uint8_t Level) {
  static uint8_t solid[NUM_STRIPS][NUM_LEDS];
  uint8_t i, j;
  int iOffset, jOffset;

  for (i = 0; i < NUM_STRIPS; i++) {
    iOffset = i * Density;
    for (j = 0; j < (NUM_LEDS / (255 / Level)); j++) {
      jOffset = j * Density;
      solid[i][j] = inoise8(x + iOffset, y + jOffset, (Density * 10));
    }
    for(j = 0; j < NUM_LEDS; j++) {
      leds[i][j] = ColorFromPalette((CRGBPalette16)earth_gp, solid[i][j]);
    }
  }
}

void Water(uint8_t Waves, uint8_t Level) {
  static uint8_t liquid[NUM_STRIPS][NUM_LEDS];
  uint8_t i, j;
  int iOffset, jOffset;

  for (i = 0; i < NUM_STRIPS; i++) {
    iOffset = i * Waves;
    for (j = 0; j < (uint8_t)((Level * NUM_LEDS) / 255); j++) {
      jOffset = j * Waves;
      liquid[i][j] = inoise8(x + iOffset, y + jOffset, z);
    }
    for (j = (uint8_t)((Level * NUM_LEDS) / 255) + 1; j < NUM_LEDS; j++) {
      liquid[i][j] = 0;
    }
    z++; // move through the Perlin noise
    for(j = 0; j < NUM_LEDS; j++) {
      leds[i][j] = ColorFromPalette((CRGBPalette16)water_gp, liquid[i][j]);
    }
  }
}

void Fire(uint8_t Sparking, uint8_t Cooling) {
  // Inspired by Fire2012 by Mark Kriegsman, July 2012
  // as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY

  static uint8_t heat[NUM_STRIPS][NUM_LEDS];
  uint8_t i, j;

  for (i = 0; i < NUM_STRIPS; i++) {
    // Step 1.  Cool down every cell a little
    for(j = 0; j < NUM_LEDS; j++) {
      heat[i][j] = qsub8(heat[i][j],  random8(0, ((Cooling * 10) / NUM_LEDS) + 2));
    }
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for(j = NUM_LEDS - 1; j >= 2; j--) {
      heat[i][j] = (heat[i][j - 1] + heat[i][j - 2] + heat[i][j - 2] ) / 3;
    }
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if(random8() < Sparking) {
      j = random8(7);
      heat[i][j] = qadd8(heat[i][j], random8(160,255));
    }
    // Step 4.  Map from heat cells to LED colors
    for(j = 0; j < NUM_LEDS; j++) {
      leds[i][j] = HeatColor(heat[i][j]);
//      leds[i][j] = ColorFromPalette((CRGBPalette16)fire_gp, heat[i][j]);
    }
  }
}

void Air(uint8_t Bubbling, uint8_t Level) {
  static uint8_t gas[NUM_STRIPS][NUM_LEDS];
  uint8_t i, j;

  for (i = 0; i < NUM_STRIPS; i++) {
    // rise all bubbles
    for(j = NUM_LEDS - 1; j >= 1; j--) {
      gas[i][j] = gas[i][j - 1];
    }
    // Generate new bubbles at random
    if(random8() < (Bubbling/8)) {
      gas[i][0] = 0;
      gas[i][1] = 127;
      gas[i][2] = 255;
    }
    for(j = 0; j < NUM_LEDS; j++) {
      leds[i][j] = ColorFromPalette((CRGBPalette16)air_gp, gas[i][j]);
    }
  }
}

void Rainbow() {
  static uint8_t Gradient;
  uint8_t i;

  if (Gradient < RAINBOW_STEP) {
    Gradient = 255;
  } else {
    Gradient -= RAINBOW_STEP;
  }

  for (i = 0; i < NUM_STRIPS; i++) {
    fill_rainbow(leds[i], NUM_LEDS, ((i * 20 + Gradient) % 255), 4);
  }
}

uint16_t ReadPot(uint8_t Channel) {
  return analogRead(Channel) / 4;
}

uint8_t ReadState(uint8_t Value) {
  return scale8(Value, 9);
}
