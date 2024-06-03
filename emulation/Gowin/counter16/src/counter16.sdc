//File Title: Timing Constraints file
create_clock -name sys_clk -period 1000 -waveform {0 500} [get_ports {CLK}] -add
