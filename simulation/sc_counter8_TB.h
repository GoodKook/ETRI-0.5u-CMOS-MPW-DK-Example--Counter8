/*******************************************************************************
Vendor: GoodKook, goodkook@gmail.com
Associated Filename: sc_counter8_TB.h
Purpose: Testbench for counter8
Revision History: Jun. 1, 2024
*******************************************************************************/
#ifndef _SC_COUNTER8_TB_H_
#define _SC_COUNTER8_TB_H_

#include <systemc.h>
#include "Vcounter8.h"
#ifdef CO_EMULATION
#include "counter8.h"
#endif

SC_MODULE(sc_counter8_TB)
{
    sc_clock            CLK;
    sc_signal<bool>     nCLR_L;
    sc_signal<bool>     nCLR_H;
    sc_signal<bool>     nLOAD_L;
    sc_signal<bool>     nLOAD_H;
    sc_signal<bool>     ENP;
    sc_signal<bool>     ENT;
    sc_signal<bool>     RCO;

    // Verilator treats all Verilog's vector as <uint32_t>
    sc_signal<uint32_t> Din_n32;
    sc_signal<uint32_t> Dout_n32;
    // Exact DUT ports' vector width
    sc_signal<sc_uint<8> > Din_n8;
    sc_signal<sc_uint<8> > Dout_n8;

    // Verilated DUT or Foreign Verilog
    Vcounter8*   u_Vcounter8;
#ifdef CO_EMULATION
    counter8*    u_counter8;
    sc_signal<sc_uint<8> >  Dout_emu;
    sc_signal<bool>         RCO_emu;
#endif

    // Convert Verilator's ports to DUT's ports
    void conv_method()
    {
        Din_n8.write((sc_uint<8>)Din_n32);
        Dout_n8.write((sc_uint<8>)Dout_n32);
    }

    void test_generator();

    sc_trace_file* fp;  // VCD file

    SC_CTOR(sc_counter8_TB) :   // Constructor
        CLK("CLK", 100, SC_NS, 0.5, 0.0, SC_NS, false)
    {
        // DUT Instantiation
        u_Vcounter8 = new Vcounter8("u_Vcounter8");
        // Binding
        u_Vcounter8->CLK(CLK);
        u_Vcounter8->nCLR_L(nCLR_L);
        u_Vcounter8->nCLR_H(nCLR_H);
        u_Vcounter8->nLOAD_L(nLOAD_L);
        u_Vcounter8->nLOAD_H(nLOAD_H);
        u_Vcounter8->ENP(ENP);
        u_Vcounter8->ENT(ENT);
        u_Vcounter8->Din(Din_n32);
        u_Vcounter8->Dout(Dout_n32);
        u_Vcounter8->RCO(RCO);

#ifdef CO_EMULATION
        u_counter8 = new counter8("u_counter8");
        // Binding
        u_counter8->CLK(CLK);
        u_counter8->nCLR_L(nCLR_L);
        u_counter8->nCLR_H(nCLR_H);
        u_counter8->nLOAD_L(nLOAD_L);
        u_counter8->nLOAD_H(nLOAD_H);
        u_counter8->ENP(ENP);
        u_counter8->ENT(ENT);
        u_counter8->Din(Din_n8);
        u_counter8->Dout(Dout_emu);
        u_counter8->RCO(RCO_emu);
#endif

        SC_THREAD(test_generator);
        sensitive << CLK;

        SC_METHOD(conv_method);
        sensitive << Din_n32 << Dout_n32;

        // VCD Trace
        fp = sc_create_vcd_trace_file("sc_counter8_TB");
        fp->set_time_unit(100, SC_PS);
        sc_trace(fp, CLK, "CLK");
        sc_trace(fp, nCLR_L, "nCLR_L");
        sc_trace(fp, nCLR_H, "nCLR_H");
        sc_trace(fp, nLOAD_L, "nLOAD_L");
        sc_trace(fp, nLOAD_H, "nLOAD_H");
        sc_trace(fp, ENP, "ENP");
        sc_trace(fp, ENT, "ENT");
        sc_trace(fp, Din_n8, "Din");
        sc_trace(fp, Dout_n8, "Dout");
        sc_trace(fp, RCO, "RCO");
#ifdef CO_EMULATION
        sc_trace(fp, Dout_emu, "Dout_emu");
        sc_trace(fp, RCO_emu, "RCO_emu");
#endif
    }

    // Destructor
    ~sc_counter8_TB()
    {}

};

#endif // _SC_CPU_6502_TOP_H_
