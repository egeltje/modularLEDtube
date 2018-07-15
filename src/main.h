#define LED_PIN0           2
#define LED_PIN1          14
#define LED_PIN2           7
#define LED_PIN3           8
#define LED_PIN4           6
#define LED_PIN5          20
#define LED_PIN6          21
#define LED_PIN7           5

#define SWITCH_PIN0       17
#define SWITCH_PIN1       18
#define SWITCH_PIN2       19
#define AD_CHANNEL8_PIN   22
#define AD_CHANNEL9_PIN   23
#define TX1_PIN            1
#define RX1_PIN            0

#define AD_CHANNEL0        9 // original channel 9
#define AD_CHANNEL1        8 // original channel 8

#define COLOR_ORDER       GRB
#define CHIPSET           WS2812
#define NUM_LEDS          60
#define NUM_STRIPS        8

#define FRAMES_PER_SECOND 60

#define FIRE_COOLING      32

CRGB leds[NUM_STRIPS][NUM_LEDS];

void Earth(int Density);
void Water(int Waving);
void Fire(int Sparking);
void Air(int Bubbling);
void Rainbow();

char ReadAnalog(char Channel);
char ReadSwitch();
