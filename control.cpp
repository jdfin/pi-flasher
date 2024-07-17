#include <cassert>
#include "mcp23x17.h"
#include "control.h"


Control::Control(Mcp23x17& mcp23x17) :
    _mcp23x17(mcp23x17)
{
    ce_oe_we(1, 1, 1);
}


Control::~Control()
{
}


void Control::ce_oe_we(int ce, int oe, int we)
{
    assert((ce == 0 || ce == 1) &&
           (oe == 0 || oe == 1) &&
           (we == 0 || we == 1));

    _mcp23x17.set_bits(ce_msk | oe_msk | we_msk,
                       (ce << ce_bit) | (oe << oe_bit) | (we << we_bit));
}
