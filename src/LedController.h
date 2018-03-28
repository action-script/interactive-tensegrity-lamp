#pragma once

#include <stdint.h>

// NeoPixel Libs
#include <Adafruit_NeoPixel.h>

// color helper
#include "HSLtoRGB.h"

#define LED_PIN 6 // digital pin controller

#define NUM_LEDS 108 //total number of leds
#define BLOCK_LEDS 18 // number of leds in row
#define LED_BARS 6 // number of led rows
#define BRIGHTNESS 220

// TODO: make it better
#define TOTAL_MERGE_INFLUENCE 400


Adafruit_NeoPixel strip = Adafruit_NeoPixel(
  NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

class LedController {
  RGBW led_colors[NUM_LEDS];
  RGBW new_color, from_color, to_color;
  uint8_t active_mode;
  uint8_t index_head; // head of the colour carousel animation (max = NUM_LEDS)
  uint16_t merge_influence; // transition to gradient
  public:
    static const uint8_t CAROUSEL_MODE = 0, GRADIENT_MODE = 1;
    LedController() {};
    void init();
    void pushPixelColor (RGBW new_color);
    void setGradientColor (RGBW from_color, RGBW to_color);
    void fillColor (RGBW new_color);
    void updateColors ();
    void displayLamp ();
};


void LedController::init() {
  /*
    This is for the Pro Trinket 5V 16MHz, you can remove these three
    lines if you are not using a Trinket
  */
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

  index_head = 0;
  merge_influence = 0;
}

void LedController::fillColor (RGBW new_color) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    led_colors[i] = new_color;
    strip.setPixelColor(
      i,
      strip.Color(
        new_color.red, new_color.green, new_color.blue, new_color.white
      )
    );
  }

  strip.show();
}

void LedController::pushPixelColor (RGBW new_color) {
  // set to CAROUSEL_MODE and save the color
  active_mode = CAROUSEL_MODE;
  this->new_color = new_color;
}

void LedController::setGradientColor (RGBW from_color, RGBW to_color) {
  // set to GRADIENT_MODE and save the color
  active_mode = GRADIENT_MODE;
  this->from_color = from_color;
  this->to_color = to_color;
}

void LedController::displayLamp () {
  for(uint16_t i = strip.numPixels()-1; i >= 0 && i < strip.numPixels(); i--) {
    strip.setPixelColor(
      i,
      strip.Color(
        led_colors[i].red,
        led_colors[i].green,
        led_colors[i].blue,
        led_colors[i].white
      )
    );
  }

  strip.show();
}

void LedController::updateColors () {
  if (active_mode == CAROUSEL_MODE) {
    // if we are on carousel, then reset the merge
    merge_influence = 0;

    // increase head and keep it on size
    index_head = index_head < NUM_LEDS ? ++index_head : NUM_LEDS;

    // CAROUSEL animation: move one pixel to right
    for(uint16_t i = index_head-1; i > 0 && i < strip.numPixels(); i--) {
      led_colors[i] = led_colors[i-1];
    }
    led_colors[0] = this->new_color;
  }
  else if (active_mode = GRADIENT_MODE) {
    // if we are on gradient, then reset the carousel head
    index_head = 0;

    // increase influence and keep it on size
    index_head = index_head < NUM_LEDS ? ++index_head : NUM_LEDS;
    merge_influence = merge_influence < TOTAL_MERGE_INFLUENCE ? ++merge_influence : TOTAL_MERGE_INFLUENCE;

    // each BAR
    for(uint8_t bar = 0; bar < LED_BARS; bar++) {

      // each LED in BAR
      for(uint8_t i = 0; i < BLOCK_LEDS; i++) {
        RGBW color = {};
        color.red = map(i, 0, BLOCK_LEDS-1, this->from_color.red, this->to_color.red);
        color.green = map(i, 0, BLOCK_LEDS-1, this->from_color.green, this->to_color.green);
        color.blue = map(i, 0, BLOCK_LEDS-1, this->from_color.blue, this->to_color.blue);
        color.white = this->from_color.white;

        // transition to gradient atimation: influence increase
        if( merge_influence < TOTAL_MERGE_INFLUENCE) {
          RGBW influenced_color = color;

          influenced_color.red = map(merge_influence, 0, TOTAL_MERGE_INFLUENCE,
            led_colors[i + (bar*BLOCK_LEDS)].red, color.red);
          influenced_color.green = map(merge_influence, 0, TOTAL_MERGE_INFLUENCE,
            led_colors[i + (bar*BLOCK_LEDS)].green, color.green);
          influenced_color.blue = map(merge_influence, 0, TOTAL_MERGE_INFLUENCE,
            led_colors[i + (bar*BLOCK_LEDS)].blue, color.blue);
          influenced_color.white = map(merge_influence, 0, TOTAL_MERGE_INFLUENCE,
            led_colors[i + (bar*BLOCK_LEDS)].white, color.white);

          color = influenced_color;
        }

        led_colors[i + (bar*BLOCK_LEDS)] = color;
      }
    }

  } // end if GRADIENT_MODE

}
