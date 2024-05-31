/*******************************************************************************
Vendor: GoodKook, goodkook@gmail.com
Associated Filename: sc_counter8_TB.cpp
Purpose: Testbench for counter8
Revision History: Jun. 1, 2024
*******************************************************************************/
#include "sc_counter8_TB.h"

void sc_counter8_TB::test_generator()
{
    nLOAD_L.write(1);
    nLOAD_H.write(1);

    ENP.write(0);
    ENT.write(0);

    nCLR_L.write(0);
    nCLR_H.write(0);
    wait(CLK.posedge_event());
    wait(CLK.posedge_event());
    wait(CLK.posedge_event());

    Din_n32.write(0x00);

    ENP.write(1);
    ENT.write(1);
    nCLR_L.write(1);
    nCLR_H.write(1);

    for (int nTest = 0; nTest <500; nTest++)
    {
        wait(CLK.negedge_event());  // avoid setup violation when timing sim.
    }
    //while(true)
    //{
    //    wait(CLK.negedge_event());  // avoid setup violation when timing sim.
    //}
    
    sc_stop();
}
    