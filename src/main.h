#define LED_PIN0           2
#define LED_PIN1          14
#define LED_PIN2           7
#define LED_PIN3           8
#define LED_PIN4           6
#define LED_PIN5          20
#define LED_PIN6          21
#define LED_PIN7           5

#define SWITCH_PIN0       22
#define SWITCH_PIN1       23
#define AD_CHANNEL3_PIN   17
#define AD_CHANNEL4_PIN   18
#define AD_CHANNEL5_PIN   19
#define TX1_PIN            1
#define RX1_PIN            0

#define POT_BRIGHTNESS     3 // original channel 3
#define POT_EFFECT1        4 // original channel 4
#define POT_EFFECT2        5 // original channel 5

#define COLOR_ORDER       GRB
#define CHIPSET           WS2812
#define NUM_LEDS          48
#define NUM_STRIPS         8

#define FRAMES_PER_SECOND 60

#define FIRE_COOLING      32
#define WATER_LEVEL       NUM_LEDS / 3 // fill tubes with 1/3th of water
#define RAINBOW_STEP       4

CRGB leds[NUM_STRIPS][NUM_LEDS];

DEFINE_GRADIENT_PALETTE( earth_gp ) {
  0,   0,   0,   0,
255, 127, 127,   0
};
DEFINE_GRADIENT_PALETTE( water_gp ) {
  0,   0,   0,   0,
  1,   0,   0, 127,
223,  31,  31, 255,
255,  63,  63, 255
};
DEFINE_GRADIENT_PALETTE( fire_gp ) {
  0,   0,   0,   0,
 63, 255,   0,   0,
127, 255, 255,   0,
255, 255, 255, 255
};
DEFINE_GRADIENT_PALETTE( air_gp ) {
  0,   0,   0,   0,
255, 127, 127, 127
};


void Earth(char Density);
void Water(char Level, char Waves);
void Fire(char Sparking, char Cooling);
void Air(char Bubbling);
void Rainbow();

char ReadPot(char Channel);
char ReadSwitch();
