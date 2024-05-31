//
// Loadable 8-bit counter
//    made of 2 LS163
//

module counter16(CLK, nCLR, nLOAD, ENP, ENT, Din, Dout, RCO);
    input           CLK;
    input           nCLR, nLOAD;
    input           ENP, ENT;
    input  [3:0]    Din;
    output [15:0]   Dout;
    output          RCO;

    wire Full_0, Full_1, Full_2;
    
    LS163 u_LS163x0(
            .nCLR(nCLR),
            .nLOAD(nLOAD),
            .Din(Din[3:0]),
            .CLK(CLK),
            .ENP(ENP),
            .ENT(ENT),
            .Dout(Dout[3:0]),
            .RCO(Full_0));

    LS163 u_LS163x1(
            .nCLR(nCLR),
            .nLOAD(nLOAD),
            .Din(4'b0000),
            .CLK(CLK),
            .ENP(ENP),
            .ENT(Full_0),
            .Dout(Dout[7:4]),
            .RCO(Full_1));

    LS163 u_LS163x2(
            .nCLR(nCLR),
            .nLOAD(nLOAD),
            .Din(4'b0000),
            .CLK(CLK),
            .ENP(ENP),
            .ENT(Full_1),
            .Dout(Dout[11:8]),
            .RCO(Full_2));

    LS163 u_LS163x3(
            .nCLR(nCLR),
            .nLOAD(nLOAD),
            .Din(4'b0000),
            .CLK(CLK),
            .ENP(ENP),
            .ENT(Full_2),
            .Dout(Dout[15:12]),
            .RCO(RCO));
endmodule
