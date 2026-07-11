RTL = rtl
TB = tb
TESTS = tests
TOP_MODULE = nand_gate
BUILD = ./build
SIM_BUILD = ./$(BUILD)/rtl_out

# cocotb config
SIM := verilator
TOPLEVEL_LANG := verilog
VERILOG_SOURCES := $(wildcard ./$(RTL)/*.sv)
COCOTB_TOPLEVEL := nand_gate
# save waves automatically
WAVES = 1
EXTRA_ARGS += --trace --timing -Wall
COCOTB_RESULTS_FILE = $(BUILD)/results.xml
SIM_ARGS += --trace-file $(BUILD)/dump.vcd
# typically overridden
TOPLEVEL := nand_gate
COCOTB_TEST_MODULES := tests.test_nand_gate

include $(shell cocotb-config --makefiles)/Makefile.sim

verilate:
	mkdir -p $(BUILD)
	mkdir -p $(SIM_BUILD)
	verilator --binary --timing --trace --threads 4 -Wall -Wno-TIMESCALEMOD -sv -O1 -cc --Mdir $(SIM_BUILD) ./$(RTL)/*.sv ./$(TB)/*.sv
	$(SIM_BUILD)/V$(TOP_MODULE) --trace --trace-file $(BUILD)/dump.vcd
