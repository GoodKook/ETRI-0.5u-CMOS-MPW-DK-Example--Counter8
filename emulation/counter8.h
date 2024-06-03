//
// Emulation Interface to "Counter8"/Cycle Accurate
// goodkook@gmail.com
//

#ifndef _COUNTER8_H_
#define _COUNTER8_H_

#include "systemc"

// Includes for accessing Arduino via serial port
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

SC_MODULE(counter8)
{
    // PORTS
    sc_in<bool>         CLK;
    sc_in<bool>         nCLR_L;
    sc_in<bool>         nCLR_H;
    sc_in<bool>         nLOAD_L;
    sc_in<bool>         nLOAD_H;
    sc_in<bool>         ENP;
    sc_in<bool>         ENT;
    sc_in<sc_uint<8> >  Din;
    sc_out<sc_uint<8> > Dout;
    sc_out<bool>        RCO;

    // Arduino Serial IF
    int fd;                 // Serial port file descriptor
    struct termios options; // Serial port setting

#define N_TX    2
#define N_RX    2

    void counter8_thread(void)
    {
        uint8_t     x, y, txPacket[N_TX], rxPacket[N_RX];

        while(true)
        {
            //-------------------------------------------------------
            wait(CLK.posedge_event());
            txPacket[0] = (uint8_t)(CLK.read()?     0x40:0x00) |
                          (uint8_t)(nCLR_L.read()?  0x20:0x00) |
                          (uint8_t)(nCLR_H.read()?  0x10:0x00) |
                          (uint8_t)(nLOAD_L.read()? 0x08:0x00) |
                          (uint8_t)(nLOAD_H.read()? 0x04:0x00) |
                          (uint8_t)(ENP.read()?     0x02:0x00) |
                          (uint8_t)(ENT.read()?     0x01:0x00);
            txPacket[1] = (uint8_t)((sc_uint<8>)(Din.read()));

            // Send to Emulator
            for (int i=0; i<N_TX; i++)
            {
                x = txPacket[i];
                while(write(fd, &x, 1)<=0)  usleep(1);
            }
            // Receive from Emulator
            for (int i=0; i<N_RX; i++)
            {
                while(read(fd, &y, 1)<=0)   usleep(1);
                rxPacket[i] = y;
            }

            Dout.write(rxPacket[0]);
            RCO.write((rxPacket[1]&0x01)? true:false);
            //-------------------------------------------------------
            wait(CLK.negedge_event());
            txPacket[0] = (uint8_t)(CLK.read()?     0x40:0x00) |
                          (uint8_t)(nCLR_L.read()?  0x20:0x00) |
                          (uint8_t)(nCLR_H.read()?  0x10:0x00) |
                          (uint8_t)(nLOAD_L.read()? 0x08:0x00) |
                          (uint8_t)(nLOAD_H.read()? 0x04:0x00) |
                          (uint8_t)(ENP.read()?     0x02:0x00) |
                          (uint8_t)(ENT.read()?     0x01:0x00);
            txPacket[1] = (uint8_t)((sc_uint<8>)(Din.read()));

            // Send to Emulator
            for (int i=0; i<N_TX; i++)
            {
                x = txPacket[i];
                while(write(fd, &x, 1)<=0)  usleep(1);
            }
            // Receive from Emulator
            for (int i=0; i<N_RX; i++)
            {
                while(read(fd, &y, 1)<=0)   usleep(1);
                rxPacket[i] = y;
            }

            Dout.write(rxPacket[0]);
            RCO.write((rxPacket[1]&0x01)? true:false);
        }
    }

    SC_CTOR(counter8) :   // Constructor
        CLK("CLK")
    {
        SC_THREAD(counter8_thread);
        sensitive << CLK;
        
        // Arduino DUT
        //fd = open("/dev/ttyACM0", O_RDWR | O_NDELAY | O_NOCTTY);
        fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
        if (fd < 0)
        {
            perror("Error opening serial port");
            return;
        }
        // Set up serial port
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        // Apply the settings
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd, TCSANOW, &options);

        // Establish Contact
        int len = 0;
        char rx;
        while(!len)
            len = read(fd, &rx, 1);
        if (rx=='A')
            write(fd, &rx, 1);
        printf("Connection established...\n");
    }

};

#endif

