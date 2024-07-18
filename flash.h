#pragma once

#include <cstdint>

class Control;
class ABus;
class DBus;


class Flash
{

    public:

        Flash(Control& ctrl, ABus& abus, DBus& dbus);

        virtual ~Flash();

        void dump(uint32_t adrs, int len);

        void get_id(uint8_t& mfg, uint8_t& dev);

        void program_byte(uint32_t adrs, uint8_t data);

        void program(uint32_t adrs, uint8_t *buf, int len);

        void erase_sector(uint32_t adrs);

        void erase_chip();

        static const int sector_size = 4096;

    private:

        static const int t_bp_us = 20;
        static const int t_se_us = 25000;
        static const int t_sce_us = 100000;

        Control& _ctrl;
        ABus& _abus;
        DBus& _dbus;

        uint8_t read(uint32_t adrs);

        void write(uint32_t adrs, uint8_t data);

}; // class Flash
