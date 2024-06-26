# -----------------------------------------------------
VERILOG_SRCS = ../source/counter8.v ../source/LS163.v
SC_SRCS      = ../simulation/sc_main8.cpp ../simulation/sc_counter8_TB.cpp
SC_TOP_H     = ../simulation/sc_counter8_TB.h ./counter8.h

SC_DEFS      = CO_EMULATION

VERILATOR    = verilator
CFLAGS       = -std=c++17
CFLAGS1      = -D$(SC_DEFS)

TOP_MODULE   = counter8
TARGET       = V$(TOP_MODULE)
TARGET_DIR   = obj_dir

export SYSTEMC			= /usr/local/systemc-3.0.0
export SYSTEMC_HOME		= $(SYSTEMC)
export SYSTEMC_INCLUDE	= $(SYSTEMC_HOME)/include
export SYSTEMC_LIBDIR	= $(SYSTEMC_HOME)/lib-linux64
export LD_LIBRARY_PATH	:=$(LD_LIBRARY_PATH):$(SYSTEMC_LIBDIR)
export CXX				= clang++
export CXXFLAGS			= -std=c++17

all :
	@echo 'Makefile for counter8 Co-Emulation: Verilog RTL(*) under SystemC Testbench'
	@echo ''
	@echo '    make build'
	@echo '        Builds executable of "counter8" with Verilated RTL-vs-Hardware and SystemC TB'
	@echo ''
	@echo '    make run'
	@echo '        Simulate counter8'
	@echo ''
	@echo '    make wave'
	@echo '        View SystemC generated waveform'
	@echo ''
	@echo '    make clean'
	@echo '        Clean working folder'
	@echo ''

build : $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/$(TARGET) : $(VERILOG_SRCS) $(SC_SRCS) $(SC_TOP_H)
	$(VERILATOR) --sc -Wall --trace --top-module $(TOP_MODULE) --exe --build \
		-CFLAGS $(CFLAGS) -CFLAGS $(CFLAGS1) \
		-CFLAGS -I../../emulation \
		$(VERILOG_SRCS) $(SC_SRCS)

lint : $(VERILOG_SRCS)
	$(VERILATOR) --sc -Wall --top-module $(TOP_MODULE) $(VERILOG_SRCS)

run : $(TARGET_DIR)/$(TARGET)
	./$(TARGET_DIR)/$(TARGET)

wave : sc_counter8_TB.vcd
	gtkwave sc_counter8_TB.vcd --save=sc_counter8_TB.gtkw &

clean :
	rm -rf $(TARGET_DIR)
	rm -f *.vcd

debug : $(TARGET_DIR)/$(TARGET)
	ddd $(TARGET_DIR)/$(TARGET)
