/*
  counter16 with SystemC Co-Emulation
*/

// Standard Emulator ------------------------------------------------
#include "PinMap_TANG_25K.h"
#include "PSCE_APIs.h"

// Co-Emulation interface -------------------------------------------
// Followings are DUT specific defs
#define DELAY_MICROS    10
#define N_RX            1 // [0]={-|CLK|nCLR|nLOAD|Din_3|Din_2|Din_1|Din_0}
#define N_TX            3 // [0]=Dout_7~0
                          // [1]=Dout_15~8
                          // [2]={-------|RCO}
#define DUT_CLK_BYTE    0
#define DUT_CLK_BITMAP  0x40

PSCE psce(DELAY_MICROS);

void setup()
{
  psce.init();  // Overclocking=114Mhz, BPS=115200
}

void loop()
{
  psce.EMU_Blinker(0x40);
  psce.RxPacket(N_RX, DUT_CLK_BYTE, DUT_CLK_BITMAP);  // CLK position: 0-th byte & 0x40
  psce.TxPacket(N_TX);
}
