#pragma once

class Mcp23x17;


class Control
{

    public:

        Control(Mcp23x17& mcp23x17);
        ~Control();

        void ce_oe_we(int ce, int oe, int we);

    private:

        static const int we_bit = 13;
        static const int oe_bit = 14;
        static const int ce_bit = 15;

        static const uint16_t we_msk = 1 << we_bit;
        static const uint16_t oe_msk = 1 << oe_bit;
        static const uint16_t ce_msk = 1 << ce_bit;

        Mcp23x17& _mcp23x17;

}; // class Control
