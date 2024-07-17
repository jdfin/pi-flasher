
#include <unistd.h>
#include <cstdint>
#include <cstdio>
#include "control.h"
#include "abus.h"
#include "dbus.h"
#include "flash.h"


void Flash::read(uint32_t adrs, uint8_t *buf, int len)
{
    _dbus.input();

    _ctrl.ce_oe_we(0, 0, 1);
    while (len-- > 0) {
        _abus.set(adrs++);
        *buf = _dbus.get();
    }
    _ctrl.ce_oe_we(1, 1, 1);
}


void Flash::write(uint32_t adrs, uint8_t *buf, int len)
{
    _ctrl.ce_oe_we(0, 1, 1);

    while (len-- > 0) {
        _abus.set(adrs++);
        _dbus.set(*buf++);
        _ctrl.ce_oe_we(0, 1, 0);
        _ctrl.ce_oe_we(0, 1, 1);
    }
    _ctrl.ce_oe_we(1, 1, 1);

    _dbus.input();
}


// 16 bytes per line
void Flash::dump(uint32_t adrs, int len)
{
    uint32_t adrs_0 = adrs & ~0xf;
    uint32_t adrs_1 = adrs + len;

    _dbus.input();

    _ctrl.ce_oe_we(0, 0, 1);
    while (adrs_0 < adrs_1) {
        if ((adrs_0 & 0xf) == 0)
            printf("%04x:", adrs_0);
        if (adrs_0 >= adrs) {
            _abus.set(adrs_0);
            printf(" %02x", _dbus.get());
        } else {
            printf(" ..");
        }
        adrs_0++;
        if ((adrs_0 & 0xf) == 0 && adrs_0 < adrs_1)
            printf("\n");
    }
    _ctrl.ce_oe_we(1, 1, 1);

    printf("\n");
}


Flash::Flash(Control& ctrl, ABus& abus, DBus& dbus) :
    _ctrl(ctrl),
    _abus(abus),
    _dbus(dbus)
{
}


Flash::~Flash()
{
}


void Flash::get_id(uint8_t& mfg, uint8_t& dev)
{
    write(0x5555, 0xaa);
    write(0x2aaa, 0x55);
    write(0x5555, 0x90);
    mfg = read(0);
    dev = read(1);
    write(0x0000, 0xf0);
}


void Flash::program_byte(uint32_t adrs, uint8_t data)
{
    write(0x5555, 0xaa);
    write(0x2aaa, 0x55);
    write(0x5555, 0xa0);
    write(adrs, data);
    usleep(t_bp_us);
}


void Flash::program(uint32_t adrs, uint8_t *buf, int len)
{
    while (len-- > 0)
        program_byte(adrs++, *buf++);
}


void Flash::erase_sector(uint32_t adrs)
{
    write(0x5555, 0xaa);
    write(0x2aaa, 0x55);
    write(0x5555, 0x80);
    write(0x5555, 0xaa);
    write(0x2aaa, 0x55);
    write(adrs, 0x30);
    usleep(t_se_us);
}


void Flash::erase_chip()
{
    write(0x5555, 0xaa);
    write(0x2aaa, 0x55);
    write(0x5555, 0x80);
    write(0x5555, 0xaa);
    write(0x2aaa, 0x55);
    write(0x5555, 0x10);
    usleep(t_sce_us);
}
