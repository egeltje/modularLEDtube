#define LED_PIN           2
#define COLOR_ORDER       GRB
#define CHIPSET           WS2812
#define NUM_LEDS          60

#define BRIGHTNESS        200
#define FRAMES_PER_SECOND 60

// For Fire2012
#define COOLING  55
#define SPARKING 120


CRGB leds[NUM_LEDS];

void Fire2012();
