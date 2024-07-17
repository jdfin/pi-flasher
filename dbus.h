#pragma once

#include <cstdint>

class Mcp23x17;

class DBus {

    public:

        DBus(Mcp23x17& mcp23x17);
        ~DBus();

        // set values
        void set(uint8_t value);

        // read and return inputs
        uint8_t get();

        void input();

        void output();

    private:

        Mcp23x17& _mcp23x17;

        bool _input;

}; // class DBus
