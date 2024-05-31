# -----------------------------------------------------
VERILOG_SRCS = ../source/counter16.v ../source/LS163.v
SC_SRCS      = sc_main16.cpp sc_counter16_TB.cpp
SC_TOP_H     = sc_counter16_TB.h
VERILATOR    = verilator
CFLAGS       = -std=c++17
#CFLAGS1      = -g

TOP_MODULE   = counter16
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
	@echo 'Makefile for counter16 Co-Simulation: Verilog RTL(*) under SystemC Testbench'
	@echo ''
	@echo '    make build'
	@echo '        Builds executable of "counter16" with Verilated RTL and SystemC TB'
	@echo ''
	@echo '    make run'
	@echo '        Simulate counter16'
	@echo ''
	@echo '    make wave'
	@echo '        View SystemC generated waveform'
	@echo ''
	@echo '    make clean'
	@echo '        Clean working folder'
	@echo ''

build : $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/$(TARGET) : $(VERILOG_SRCS) $(SC_SRCS) $(SC_TOP_H)
	$(VERILATOR) --sc -Wall --top-module $(TOP_MODULE) --exe --build \
		-CFLAGS $(CFLAGS) \
		$(VERILOG_SRCS) $(SC_SRCS)

lint : $(VERILOG_SRCS)
	$(VERILATOR) --sc -Wall --top-module $(TOP_MODULE) $(VERILOG_SRCS)

run : $(TARGET_DIR)/$(TARGET)
	./$(TARGET_DIR)/$(TARGET)

wave : sc_counter16_TB.vcd
	gtkwave sc_counter16_TB.vcd --save=sc_counter16_TB.gtkw &

clean :
	rm -rf $(TARGET_DIR)
	rm -f *.vcd

debug : $(TARGET_DIR)/$(TARGET)
	ddd $(TARGET_DIR)/$(TARGET)

