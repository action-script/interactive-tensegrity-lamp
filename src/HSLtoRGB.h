#pragma once

#include <stdint.h>

/*
 * color value RGB from HSL
 */

// color data structure
struct RGBW {
  public:
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t white;
};

float hue2rgb (float p, float q, float t){
  if(t < 0) t += 1;
  if(t > 1) t -= 1;
  if(t < 1/6.0) return p + (q - p) * 6 * t;
  if(t < 1/2.0) return q;
  if(t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
  return p;
}

RGBW HSLtoRGB (uint8_t hue, uint8_t saturation, uint8_t lightness) {
  float h = hue / 100.0;
  float s = saturation / 100.0;
  float l = lightness / 100.0;

  RGBW color;
  float r, g, b;

  if(s == 0) {
      r = g = b = l;
  }
  else {
      float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
      float p = 2 * l - q;
      r = hue2rgb(p, q, h + 1/3.0);
      g = hue2rgb(p, q, h);
      b = hue2rgb(p, q, h - 1/3.0);
  }

  color.red =   r * 255;
  color.green = g * 255;
  color.blue =  b * 255;

  return color;
}
