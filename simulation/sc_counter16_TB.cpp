/*******************************************************************************
Vendor: GoodKook, goodkook@gmail.com
Associated Filename: sc_counter16_TB.cpp
Purpose: Testbench for counter16
Revision History: Jun. 1, 2024
*******************************************************************************/
#include "sc_counter16_TB.h"

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
        wait(CLK.negedge_event());  // avoid setup violation when timing sim.
    }
    //while(true)
    //{
    //    wait(CLK.negedge_event());  // avoid setup violation when timing sim.
    //}
    
    sc_stop();
}
