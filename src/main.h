#define LED_PIN0           2
#define LED_PIN1          14
#define LED_PIN2           7
#define LED_PIN3           8
#define LED_PIN4           6
#define LED_PIN5          20
#define LED_PIN6          21
#define LED_PIN7           5

#define TX1_PIN            1
#define RX1_PIN            0

#define POT_EFFECT_MOD1    3 // original channel 3 pin 18
#define POT_EFFECT_MOD2    4 // original channel 4 pin 19
#define POT_EFFECT_MOD3    5 // original channel 5 pin 19
#define POT_EFFECT         8 // original channel 8 Pin 17
#define POT_SPARE          9 // original channel 9 pin 23

#define COLOR_ORDER        GRB
#define CHIPSET            WS2812
#define NUM_LEDS           48
#define NUM_STRIPS         8

#define FRAME_DELAY        40

#define WATER_NOISE_SCALE 64
#define EARTH_NOISE_SCALE 64
#define RAINBOW_STEP       4

CRGB leds[NUM_STRIPS][NUM_LEDS];

DEFINE_GRADIENT_PALETTE( earth_gp ) {
  0,   0,   0,   0,
127,  31,   0,   0,
255, 127, 127,   0
};
DEFINE_GRADIENT_PALETTE( water_gp ) {
  0,   0,   0,   0,
  1,   0,  63,   0,
127,   0,  63, 127,
223,   0, 223, 127,
255, 127, 255, 255
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

// The 32bit version of our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;

uint8_t State;
uint8_t Param1, Param2, Param3, Param4;
uint16_t tempParam1, tempParam2, tempParam3, tempParam4;
uint8_t ParamStep;

void Light();
void Earth(uint8_t Density, uint8_t Level);
void Water(uint8_t Waves, uint8_t Level);
void Fire(uint8_t Sparking, uint8_t Cooling);
void Air(uint8_t Bubbling, uint8_t Level);
void Rainbow();

uint16_t ReadPot(uint8_t Channel);
uint8_t ReadState(uint8_t Value);
