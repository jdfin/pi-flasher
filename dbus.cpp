#include <cstdint>
//#include <cstdio>
#include "mcp23x17.h"
#include "dbus.h"


DBus::DBus(Mcp23x17& mcp23x17) :
           _mcp23x17(mcp23x17),
           _input(true)
{
}


DBus::~DBus()
{
}


// set values
void DBus::set(uint8_t data)
{
    //printf("DBus::set: data=0x%02x\n", int(data));
    _mcp23x17.set_bits(0x00ff, uint16_t(data));
    output();
}


// read and return inputs
uint8_t DBus::get()
{
    input();
    uint16_t data;
    _mcp23x17.get_bits(0x00ff, data);
    //printf("DBus::get: data=0x%02x\n", int(data));
    return data;
}


void DBus::input()
{
    if (!_input) {
        //printf("DBus::input\n");
        _mcp23x17.set_dirs(0x00ff, 0x00ff);
        _input = true;
    }
}


void DBus::output()
{
    if (_input) {
        //printf("DBus::output\n");
        _mcp23x17.set_dirs(0x00ff, 0x0000);
        _input = false;
    }
}
