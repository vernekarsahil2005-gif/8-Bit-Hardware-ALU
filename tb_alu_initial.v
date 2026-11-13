// File: tb_alu_initial.v
// Initial AI-Generated Testbench

`timescale 1ns / 1ps

module tb_alu_initial;

    // Inputs
    reg [7:0] InputA;
    reg [7:0] InputB;
    reg [2:0] OpCode;

    // Outputs
    wire [15:0] OutALU;
    wire COut;

    // Instantiate the Unit Under Test (UUT)
    alu_8bit uut (
        .InputA(InputA),
        .InputB(InputB),
        .OpCode(OpCode),
        .OutALU(OutALU),
        .COut(COut)
    );

    initial begin
        // Initialize Inputs
        InputA = 0;
        InputB = 0;
        OpCode = 0;

        $monitor("Time=%0t | A=%d B=%d Op=%b | OutALU=%d (0x%h) COut=%b", 
                 $time, InputA, InputB, OpCode, OutALU, OutALU, COut);

        #10;
        
        // Case 1: InputA = 45, InputB = 38
        InputA = 45; InputB = 38;
        
        OpCode = 3'b000; #10; // Addition
        OpCode = 3'b001; #10; // Subtraction
        OpCode = 3'b110; #10; // Logical OR
        OpCode = 3'b111; #10; // Logical XOR

        // Case 2: InputA = 49, InputB = 10
        InputA = 49; InputB = 10;
        
        OpCode = 3'b010; #10; // Multiplication
        OpCode = 3'b011; #10; // Left Shift
        OpCode = 3'b100; #10; // Right Shift
        OpCode = 3'b101; #10; // Logical AND

        #10;
        $finish;
    end
      
endmodule