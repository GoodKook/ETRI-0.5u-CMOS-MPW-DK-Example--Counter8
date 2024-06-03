/*******************************************************************************
Vendor: GoodKook, goodkook@gmail.com
Associated Filename: sc_counter16_TB.h
Purpose: Testbench for counter16
Revision History: Jun. 1, 2024
*******************************************************************************/
#ifndef _SC_COUNTER16_TB_H_
#define _SC_COUNTER16_TB_H_

#include <systemc.h>
#include "Vcounter16.h"
#ifdef CO_EMULATION
#include "counter16.h"
#endif

SC_MODULE(sc_counter16_TB)
{
    sc_clock            CLK;
    sc_signal<bool>     nCLR;
    sc_signal<bool>     nLOAD;
    sc_signal<bool>     ENP;
    sc_signal<bool>     ENT;
    sc_signal<bool>     RCO;

    // Verilator treats all Verilog's vector as <uint32_t>
    sc_signal<uint32_t>     Din_n32;
    sc_signal<uint32_t>     Dout_n32;
    // Exact DUT ports' vector width
    sc_signal<sc_uint<4> >  Din_n4;
    sc_signal<sc_uint<16> > Dout_n16;
    // Verilated DUT or Foreign Verilog
    Vcounter16*   u_Vcounter16;
#ifdef CO_EMULATION
    counter16*    u_counter16;
    sc_signal<sc_uint<16> > Dout_emu;
    sc_signal<bool>         RCO_emu;
#endif

    // Convert Verilator's ports to DUT's ports
    void conv_method()
    {
        Din_n4.write((sc_uint<4>)Din_n32);
        Dout_n16.write((sc_uint<16>)Dout_n32);
    }

    void test_generator();
    void monitor();
    
    sc_trace_file* fp;  // VCD file

    SC_CTOR(sc_counter16_TB) :   // Constructor
        CLK("CLK", 100, SC_NS, 0.5, 0.0, SC_NS, false)
    {
        // DUT Instantiation
        u_Vcounter16 = new Vcounter16("u_Vcounter16");
        // Binding
        u_Vcounter16->CLK(CLK);
        u_Vcounter16->nCLR(nCLR);
        u_Vcounter16->nLOAD(nLOAD);
        u_Vcounter16->ENP(ENP);
        u_Vcounter16->ENT(ENT);
        u_Vcounter16->Din(Din_n32);
        u_Vcounter16->Dout(Dout_n32);
        u_Vcounter16->RCO(RCO);

#ifdef CO_EMULATION
        u_counter16 = new counter16("u_counter16");
        // Binding
        u_counter16->CLK(CLK);
        u_counter16->nCLR(nCLR);
        u_counter16->nLOAD(nLOAD);
        u_counter16->Din(Din_n4);
        u_counter16->Dout(Dout_emu);
        u_counter16->RCO(RCO_emu);
#endif

        SC_THREAD(test_generator);
        sensitive << CLK;

        SC_THREAD(monitor);
        sensitive << CLK;

        SC_METHOD(conv_method);
        sensitive << Din_n32 << Dout_n32;

        // VCD Trace
        fp = sc_create_vcd_trace_file("sc_counter16_TB");
        fp->set_time_unit(100, SC_PS);
        sc_trace(fp, CLK,       "CLK");
        sc_trace(fp, nCLR,      "nCLR");
        sc_trace(fp, nLOAD,     "nLOAD");
        sc_trace(fp, ENP,       "ENP");
        sc_trace(fp, ENT,       "ENT");
        sc_trace(fp, Din_n4,    "Din");
        sc_trace(fp, Dout_n16,  "Dout");
        sc_trace(fp, RCO,       "RCO");
#ifdef CO_EMULATION
        sc_trace(fp, Dout_emu,  "Dout_emu");
        sc_trace(fp, RCO_emu,   "RCO_emu");
#endif
    }

    // Destructor
    ~sc_counter16_TB()
    {}

};

#endif // _SC_CPU_6502_TOP_H_
