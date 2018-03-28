# Interactive tensegrity lamp

Movement responsive lamp. Designed as a minimal tensegrity sculpture, changes the configuration of colors, reacting to the orientation and movements of the structure.

Check the complete instructions and build process at the [instructables] page:

### Installation
This is an Arduino Project configured to run with PlatformIO. In order to run the code, you have to get the [PlatformIO Core].  

The project is designed to run on an Adafruit Pro Trinket 5V/16MHz (USB).

The hardware design includes:
 - Adafruit Pro Trinket - 5V
 - Adafruit LIS3DH Triple-Axis Accelerometer
 - Adafruit NeoPixel Digital RGBW LED Strip - White PCB 60 LED/m

PlatformIO will automatically installed the corresponding libraries when run the program.

### Run on Linux
Pro Trinket is not supported on Linux operating system at this time - try Mac OS or Windows! However, you can try the following - it does work for some computers  
Linux is fairly picky about who can poke and prod at the USB port. You can always run avrdude or Arduino IDE as root, which will make sure you have the proper permissions. If you want to be super-cool you can add a udev rule which will let any user (who is not root) connect to the USBtiny driver. That way you don't have to be root all the time!


License
----
Copyright (c) 2016 Nuño de la Serna  
Licensed under the Apache License Version 2.0

   [instructables]: <http://www.instructables.com/id/Interactive-Led-Lamp-Tensegrity-Structure-Arduino/>
   [PlatformIO Core]: <http://docs.platformio.org/en/latest/installation.html>
