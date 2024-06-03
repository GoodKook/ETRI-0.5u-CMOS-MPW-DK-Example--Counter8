/*******************************************************************************
Vendor: GoodKook, goodkook@gmail.com
Associated Filename: sc_counter16_TB.cpp
Purpose: Testbench for counter16
Revision History: Jun. 1, 2024
*******************************************************************************/
#include "sc_counter16_TB.h"

#include <iostream>
#include <iomanip>

void sc_counter16_TB::test_generator()
{
    nLOAD.write(1);

    ENP.write(0);
    ENT.write(0);

    nCLR.write(0);
    wait(CLK.posedge_event());
    wait(CLK.posedge_event());
    wait(CLK.posedge_event());

    Din_n32.write(0x00);

    ENP.write(1);
    ENT.write(1);
    nCLR.write(1);

    for (int nTest = 0; nTest <66000; nTest++)
    {
        wait(CLK.posedge_event());
    }
    
    sc_stop();
}

void sc_counter16_TB::monitor()
{
    sc_time t(1, SC_NS);
#ifdef CO_EMULATION
    while(true)
    {
        wait(CLK.posedge_event());
        if ((int)Dout_emu.read() != (int)Dout_n32.read())
        {
            cout    << "Error at "   << sc_time_stamp() << ": "
                    << std::hex
                    << "Dout=0x"     << Dout_n16.read() << " "
                    << "Dout_emu=0x" << Dout_emu.read() << endl;
        }
    }
#endif
}
