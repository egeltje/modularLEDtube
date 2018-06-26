#define LED_PIN0          2
#define LED_PIN1          14
#define LED_PIN2          7
#define LED_PIN3          8
#define LED_PIN4          6
#define LED_PIN5          20
#define LED_PIN6          21
#define LED_PIN7          5

#define BUTTON_PIN0       0
#define BUTTON_PIN1       1
#define BUTTON_PIN2       23
#define BUTTON_PIN3       22
#define BUTTON_PIN4       19
#define BUTTON_PIN5       18
#define BUTTON_PIN6       17

#define COLOR_ORDER       GRB
#define CHIPSET           WS2812
#define NUM_LEDS          60
#define NUM_STRIPS        8

#define BRIGHTNESS        127
#define FRAMES_PER_SECOND 60

// For Fire2012
#define COOLING  75       // 55
#define SPARKING 120      // 120

CRGB leds[NUM_STRIPS][NUM_LEDS];

void Light();
void Fire();
