module ram(
  input clk,

  input[31:0] addrBus,
  inout[31:0] dataBus,

  // if both enabled, RAM will do nothing
  input readRam,
  input writeRam
);

  localparam RAM_BYTES = 512 * 1024; // 512 KiB of RAM
  localparam RAM_WORDS = RAM_BYTES/4;

  logic [31:0] ram [0:RAM_WORDS-1] = '{default:'0};

  assign dataBus = (readRam && !writeRam) ? ram[addrBus] : 32'bz;

  always @(negedge clk) begin

    if (writeRam && !readRam) begin
      ram[addrBus] <= dataBus;
    end

  end

endmodule
