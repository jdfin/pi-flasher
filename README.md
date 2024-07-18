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
