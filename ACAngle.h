// AC Libs
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>

// PINS
// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10

Adafruit_LIS3DH lis = Adafruit_LIS3DH(LIS3DH_CS, LIS3DH_MOSI, LIS3DH_MISO, LIS3DH_CLK);

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

class ACAngle {
  public:
    ACAngle(){};
    void init();
    void getValues (uint16_t *roll, uint16_t *pitch);
};


void ACAngle::init() {
  //Serial.println("LIS3DH test!");
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    //Serial.println("Couldnt start");
  }
  //Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_4_G);  // 2, 4, 8 or 16 G!
  
  //Serial.print("Range: "); Serial.print(2 << lis.getRange()); Serial.println("G");
}

void ACAngle::getValues(uint16_t *roll, uint16_t *pitch) {
    lis.read();      // get X Y and Z data at once
    
    // angle calculation
    *roll = 180  + atan2(-lis.x, lis.z) * 180/ PI;
    *pitch = 90 + atan2(lis.y, sqrt( pow(lis.x, 2) + pow(lis.z, 2) )) * 180/ PI;
}
