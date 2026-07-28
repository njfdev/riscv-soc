module cpu_top #(
  parameter int ADDR_WIDTH,
  parameter int RESET_ADDR
) (
  input clk,

  output[31:0] addrBus,
  inout[31:0] dataBus,

  output memRead,
  output memWrite,

  input reset
);

  /**************** Instruction Decode ****************/

  wire isLUI    = instr[6:2] == 5'b01101;
  wire isAUIPC  = instr[6:2] == 5'b00101;
  wire isJAL    = instr[6:2] == 5'b11011;
  wire isJALR   = instr[6:2] == 5'b11001;
  wire isBranch = instr[6:2] == 5'b11000;
  wire isLoad   = instr[6:2] == 5'b00000;
  wire isStore  = instr[6:2] == 5'b01000;
  wire isALUImm = instr[6:2] == 5'b00100;
  wire isALUReg = instr[6:2] == 5'b01100;
  wire isSystem = instr[6:2] == 5'b11100;

  wire [11:0] immI = instr[31:20];
  wire [11:0] immS = {instr[31:25], instr[11:7]};
  wire [11:0] immB = {instr[31], instr[7], instr[30:25], instr[11:8]}; // remember, this actually represents bits 12:1
  wire [19:0] immU = instr[31:12];
  wire [19:0] immJ = {instr[31], instr[19:12], instr[20], instr[30:21]}; // remember, this actually represents bits 20:1

  
  /************************ ALU ************************/

  

  logic [31:0] regFile [0:31] /* verilator public_flat_rw */ = '{default:'0};

  // doesn't include system, store, or branch instructions
  wire [4:0] rdIndex = instr[11:7];
  // doesn't include LUI, AUIPC, JAL, or system instructions
  wire [4:0] rs1Index = instr[19:15];
  // only for branch, store, and ALU (non-immediate) instructions
  wire [4:0] rs2Index = instr[24:20];
  
  logic [31:0] instr;

  /********************* PC Logic **********************/ 


  logic [ADDR_WIDTH-1:0] pc = (ADDR_WIDTH)'(RESET_ADDR);

  wire [ADDR_WIDTH-1:0] jalOffset = $signed({{(ADDR_WIDTH-21){immJ[19]}}, immJ[19:0], 1'b0});

  wire [ADDR_WIDTH-1:0] nextPc = isJAL ? pc + jalOffset : pc + 4;

  /******************* Decode Logic ********************/

  localparam FETCH_STATE = 0;
  localparam EXECUTE_STATE = 1;

  logic state = 0;

  // when is fetch, load memory to instr
  assign memRead = state == FETCH_STATE;
  assign addrBus = state == FETCH_STATE ? {(32-ADDR_WIDTH)'(0), pc} : 'bz;

  always @(posedge clk) begin
    case (state)

      FETCH_STATE: begin
        instr <= dataBus;

        state <= EXECUTE_STATE;
      end

      EXECUTE_STATE: begin

        // update rd if necessary
        if (rdIndex != 0) begin
          case (1)

            isJAL: begin
              /* verilator lint_off WIDTHEXPAND */
              regFile[rdIndex] = pc+4;
              /* verilator lint_on WIDTHEXPAND */
            end

          endcase
        end

        pc <= nextPc;
        state <= FETCH_STATE;
      end

    endcase
  end

endmodule
