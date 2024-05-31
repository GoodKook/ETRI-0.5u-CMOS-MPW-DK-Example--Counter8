//
// Loadable 8-bit counter
//    made of 2 LS163
//

module counter8(CLK, nCLR_L, nCLR_H, nLOAD_L, nLOAD_H, ENP, ENT, Din, Dout, RCO);
    input           CLK;
    input           nCLR_L, nCLR_H;
    input           nLOAD_L, nLOAD_H;
    input           ENP, ENT;
    input  [7:0]    Din;
    output [7:0]    Dout;
    output          RCO;

    wire Full;
    
    LS163 u_LS163xL(
            .nCLR(nCLR_L),
            .nLOAD(nLOAD_L),
            .Din(Din[3:0]),
            .CLK(CLK),
            .ENP(ENP),
            .ENT(ENT),
            .Dout(Dout[3:0]),
            .RCO(Full));

    LS163 u_LS163xH(
            .nCLR(nCLR_H),
            .nLOAD(nLOAD_H),
            .Din(Din[7:4]),
            .CLK(CLK),
            .ENP(ENP),
            .ENT(Full),
            .Dout(Dout[7:4]),
            .RCO(RCO));

endmodule
