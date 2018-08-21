#include <Arduino.h>
#include <FastLED.h>
#include <stdint.h>
#include "main.h"

void setup() {
  // configure serial ports
  Serial.begin(115200);
  Serial1.begin(9600);

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

  // Initialize our coordinates to some random values
  x = random16();
  y = random16();
  z = random16();

  // configure switches and potentiometers
  pinMode(SWITCH_PIN0, INPUT_PULLUP);
  pinMode(SWITCH_PIN1, INPUT_PULLUP);
}

void loop()
{
  uint8_t State = 0;
  uint8_t Param1 = 0;
  uint8_t Param2 = 0;
  uint8_t Param3 = 0;
  // Add entropy to random number generator; we use a lot of it.
//  random16_add_entropy(random());

  EVERY_N_MILLISECONDS(FRAME_DELAY) {
    Param1 = ReadPot(POT_EFFECT1);
    Param2 = ReadPot(POT_EFFECT2);
    Param3 = ReadPot(POT_BRIGHTNESS);

    Serial.print("Param1: ");
    Serial.println(Param1);
    Serial.print("Param2: ");
    Serial.println(Param2);
    Serial.print("Param3: ");
    Serial.println(Param3);
    Serial.println("----------");

    switch (ReadSwitch()) {  // run simulation frame depending on state
      case 0:
        if (State != 0) {
          State = 0;
        }
        Earth(Param1, Param2);
        break;
      case 1:
        if (State != 1) {
          State = 1;
        }
        Water(Param1, Param2);
        break;
      case 2:
        if (State != 2) {
          State = 2;
        }
        Fire(Param1, Param2);
        break;
      case 3:
        if (State != 3) {
          State = 3;
        }
        Air(Param1, Param2);
        break;
      default:
        Rainbow();
    }

    FastLED.setBrightness(Param3);
    FastLED.show(); // display this frame
  }
}

void Earth(uint8_t Density, uint8_t Level) {
  static uint8_t solid[NUM_STRIPS][NUM_LEDS];
  uint8_t i, j;
  int iOffset, jOffset;

  for (i = 0; i < NUM_STRIPS; i++) {
    iOffset = i * EARTH_NOISE_SCALE;
    for (j = 0; j < (NUM_LEDS / (255 / Level)); j++) {
      jOffset = j * EARTH_NOISE_SCALE;
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
    iOffset = i * WATER_NOISE_SCALE;
    for (j = 0; j < (uint8_t)((Level * NUM_LEDS)/ MAX_POT_READING); j++) {
      jOffset = j * WATER_NOISE_SCALE;
      liquid[i][j] = inoise8(x + iOffset, y + jOffset, z);
    }
    for (j = (uint8_t)((Level * NUM_LEDS) / MAX_POT_READING) + 1; j < NUM_LEDS; j++) {
      liquid[i][j] = 0;
    }
    z = z + 1;
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
      leds[i][j] = HeatColor(heat[i][j]);
//      leds[i][j] = ColorFromPalette((CRGBPalette16)fire_gp, heat[i][j]);
    }
  }
}

void Air(uint8_t Bubbling, uint8_t Level) {
  static uint8_t gas[NUM_STRIPS][NUM_LEDS];
  uint8_t i, j;

  for (i = 0; i < NUM_STRIPS; i++) {
    for(j = 0; j < NUM_LEDS; j++) {
      gas[i][j + 1] = gas[i][j];
    }
    if(random8() < Bubbling) {
      j = random8(7);
      gas[i][j] = qadd8(gas[i][j], random8(160,255));
    }
    for(j = 0; j < NUM_LEDS; j++) {
      leds[i][j] = ColorFromPalette((CRGBPalette16)air_gp, gas[i][j]);
    }
  }
}

void Rainbow() {
  static uint8_t Gradient;

  if (Gradient < RAINBOW_STEP) {
    Gradient = 255;
  } else {
    Gradient -= RAINBOW_STEP;
  }

  for (uint8_t i = 0; i < NUM_STRIPS; i++) {
    fill_rainbow(leds[i], NUM_LEDS, Gradient, 4);
  }
}

uint8_t ReadPot(uint8_t Channel) {
  //return 127;
   return analogRead(Channel);
}

uint8_t ReadSwitch() {
  //return ((!digitalRead(SWITCH_PIN0) * 1) + (!digitalRead(SWITCH_PIN1) * 2));
  return 1;
}
