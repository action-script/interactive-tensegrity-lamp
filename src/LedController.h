// NeoPixel Libs
#include <Adafruit_NeoPixel.h>
#define LED_PIN 6 // digital pin controller

#define NUM_LEDS 108 //total number of leds
#define BLOCK_LEDS 18 // number of leds in row
#define LED_BARS 6 // number of led rows
#define BRIGHTNESS 220


Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

class LedController {
  RGBW led_colors[NUM_LEDS];
  uint8_t active_mode;
  public:
    static const uint8_t CAROUSEL_MODE = 0, OPPOSITE_MODE = 1;
    LedController(){};
    void init();
    void addColor (RGBW new_color);
    void addOppositeColor (RGBW new_color);
    void fillColor (RGBW new_color);
    void updateColors ();
    void displayLamp ();
};


void LedController::init() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // init hardware led strip
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
  
  // init led array colors
  fillColor({0,0,0,0});

  active_mode = CAROUSEL_MODE;
}

void LedController::fillColor (RGBW new_color) {  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    led_colors[i] = new_color;
    strip.setPixelColor(
      i,
      strip.Color(new_color.red, new_color.green, new_color.blue, new_color.white)
    );
  }

  strip.show();
}

void LedController::addColor (RGBW new_color) {
  led_colors[0] = new_color;
  active_mode = CAROUSEL_MODE;
}

void LedController::addOppositeColor (RGBW new_color) {
  led_colors[strip.numPixels()-1] = new_color;
  active_mode = OPPOSITE_MODE;
}

void LedController::displayLamp () {
  for(uint16_t j = strip.numPixels()-1; j >= 0 && j < strip.numPixels(); j--) {
    strip.setPixelColor(
      j,
      strip.Color(
        led_colors[j].red,
        led_colors[j].green,
        led_colors[j].blue,
        led_colors[j].white
      )
    );
  }
    
  strip.show();
}

void LedController::updateColors () {
  if (active_mode == CAROUSEL_MODE)
    for(uint16_t j = strip.numPixels()-1; j >= 0 && j < strip.numPixels(); j--) {
      if (j > 0)  led_colors[j] = led_colors[j-1];
    }
    
  else if (active_mode = OPPOSITE_MODE)
  /*
    for(uint16_t j = 0; j < BLOCK_LEDS; j++) {
      for(uint16_t k = 0; k < LED_BARS; k++) {
        if (j < BLOCK_LEDS / 2) //if (j < strip.numPixels() / 2)
          led_colors[j + (k*BLOCK_LEDS)] = led_colors[0];
        else 
          led_colors[j + (k*BLOCK_LEDS)] = led_colors[strip.numPixels()-1];
      }
    }
  for(uint16_t j = strip.numPixels()-2; j >= 1; j--) {
    if (j < strip.numPixels() / 2)
      led_colors[j] = led_colors[0];
    else 
      led_colors[j] = led_colors[strip.numPixels()-1];
  }
  
    
  for(uint16_t j = strip.numPixels()-2; j >= 1; j--) {
    RGBW color = led_colors[j];
    color.red = map(j, 1, strip.numPixels()-2, led_colors[0].red, led_colors[strip.numPixels()-1].red);
    color.green = map(j, 1, strip.numPixels()-2, led_colors[0].green, led_colors[strip.numPixels()-1].green);
    color.blue = map(j, 1, strip.numPixels()-2, led_colors[0].blue, led_colors[strip.numPixels()-1].blue);
    led_colors[j] = color;
  }
  */
    
  for(uint16_t j = 0; j < BLOCK_LEDS; j++) {
    for(uint16_t k = 0; k < LED_BARS; k++) {
      RGBW color = {};
      color.red = map(j, 0, BLOCK_LEDS, led_colors[0].red, led_colors[strip.numPixels()-1].red);
      color.green = map(j, 0, BLOCK_LEDS, led_colors[0].green, led_colors[strip.numPixels()-1].green);
      color.blue = map(j, 0, BLOCK_LEDS, led_colors[0].blue, led_colors[strip.numPixels()-1].blue);
      color.white = led_colors[0].white;
      led_colors[j + (k*BLOCK_LEDS)] = color;
    }
  }
}

