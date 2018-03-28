#include <stdint.h>

// AC
#include "ACAngle.h"
ACAngle AC;

// LEDS
#include "LedController.h"
LedController Leds;

// Times
#define AC_LOOP 30
#define LED_REFRESH 15

// Complementary color config
#define OPPOSITE_ANGLE 120

unsigned int ac_timer;
unsigned int leds_timer;

uint16_t roll = 0;
uint16_t pitch = 0;

void setup() {
  AC.init();

  Leds.init();

  // initialize timers
  ac_timer = leds_timer = millis();
}

void loop() {
  if (millis() - ac_timer >= AC_LOOP) {
    AC.getValues(&roll, &pitch);

    if (pitch < 75) {
      RGBW new_color = HSLtoRGB(
        map(roll, 0, 360, 0, 100),
        map(min(pitch, 40), 40, 0, 100, 0), // saturation
        map(min(pitch, 30), 30, 0, 50, 0) // lightness
      );

      RGBW opposite_color = HSLtoRGB(
        map( (roll + OPPOSITE_ANGLE) % 360 , 0, 360, 0, 100),
        map(min(pitch, 40), 40, 0, 100, 0), // saturation
        map(min(pitch, 30), 30, 0, 50, 0) // lightness
      );

      new_color.white = opposite_color.white = map(min(pitch, 30), 30, 0, 0, 255);

      Leds.setGradientColor(new_color, opposite_color);
    }

    else {
      RGBW new_color = HSLtoRGB(
        map(roll, 0, 360, 0, 100),
        map(max(pitch, 110), 110, 180, 100, 0), // saturation
        map(max(pitch, 160), 160, 180, 50, 0) // lightness
      );

      new_color.white = map(max(pitch, 100), 100, 180, 0, 255);

      Leds.pushPixelColor(new_color);
    }

    ac_timer = millis();
  }

  if (millis() - leds_timer >= LED_REFRESH) {
    Leds.updateColors();
    Leds.displayLamp();

    leds_timer = millis();
  }
}
