module soc (
  input clk
);

  parameter RESET_ADDR = 32'h00000000;
  parameter ADDR_WIDTH = 24;

  wire[31:0] tbData /* verilator public_flat_rw */;
  wire tbDataEnable /* verilator public_flat_rw */;

  assign dataBus = tbDataEnable ? tbData : 32'bz;

  wire[31:0] addrBus /* verilator public_flat_rw */;
  wire[31:0] dataBus /* verilator public_flat_rw */;
  
  wire memRead /* verilator public_flat_rw */;
  wire memWrite /* verilator public_flat_rw */;

  ram ram0 (
    .clk (clk),

    .addrBus (addrBus),
    .dataBus (dataBus),

    .readRam (memRead),
    .writeRam (memWrite)
  );


endmodule
