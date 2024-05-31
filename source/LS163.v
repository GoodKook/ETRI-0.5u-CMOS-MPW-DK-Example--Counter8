//
// Loadable 8-bit counter
//    made of 2 LS163
//

module LS163(nCLR, nLOAD, Din, CLK, ENP, ENT , Dout, RCO);
    input           CLK, nCLR, nLOAD;
    input           ENP, ENT;
    input  [3:0]    Din;
    output [3:0]    Dout;
    output          RCO;

    reg [3:0] cnt;
    always @(posedge CLK or negedge nCLR)
    begin
        if (!nCLR)
            cnt <= 0;
        else
        begin
            if (!nLOAD)
                cnt <= Din;
            else if (ENP && ENT)
                cnt <= cnt + 1;
        end
    end
    
    assign RCO = cnt[3] & cnt[2] & cnt[1] & cnt[0];   // Error_1
    //assign RCO = cnt[3] & cnt[2] & cnt[1] & cnt[0] & ENT;
    assign Dout = cnt;
endmodule

