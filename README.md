With required hardware, program an Intel-hex file to a flash chip.

# Hardware

The schematic is in [pi-flasher_v7.pdf](pi-flasher_v7.pdf).
The connections to the Pi are off to the right in the schematic:
```
GND, 5V, SPI0_CE0, SPI0_SCLK, SPI0_MISO, SPI0_MOSI
```
Powering it from the Pi might depend on having a good 5V supply.

A breadboarded one (running with a pi5) is shown in
[pi-flasher.jpg](pi-flasher.jpg).

Basically, a Raspberry Pi uses a pair of MCP23S17s as GPIO expanders, and those
drive the flash chip's address, data, and control pins. All hardware except the
Pi runs at 5V; there is a 74AHCT125 used for high-speed 3.3V -> 5V translation,
and a voltage divider for 5V -> 3.3V translation. The bidirectional level
shifters like you use for I2C didn't work at high speeds for SPI. The idea for
using the 74AHCT125 for level shifting came from Adafruit
([they sell them](https://www.adafruit.com/product/1787) for that purpose).

# Software

Starting with a newly-flashed Raspberry Pi:
```
$ sudo apt update
$ sudo apt full-upgrade
$ sudo raspi-config
(interface options / enable SPI)
$ sudo apt install git cmake

$ git clone git@github.com:jdfin/pi-flasher.git
$ cd pi-flasher/
$ mkdir build
$ cd build
$ cmake ..
$ make
```
Run it:
```
./flash -c                  erases the chip
./flash -d 0x100 -l 64      dumps 64 bytes starting at 0x100
./flash -e 0x1000           erases the sector at 0x1000
./flash -i                  retrieves and shows the chip IDs
./flash -p my_image.ihx     programs the file to flash
```
Multiple arguments work in a logical order, e.g.
```
./flash -c -p my_image.ihx -d 0 -l 512
```
will erase the chip, program the file, then dump.

# Pieces

Perhaps-interesting pieces if you don't care about programming flash:
- example of SPI from a Raspberry Pi
- example of 5V GPIO with the 3.3V Pi
- using an MCP23S17 for Pi GPIO expansion
  - this has worked with the I2C version of the chip; hence the Mcp23x17Bus and Mc23x17Spi classes
- reading an "Intel" hex file
