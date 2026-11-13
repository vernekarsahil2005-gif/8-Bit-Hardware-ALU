// File: tb_alu_refined.v
// Refined Self-Checking Testbench with Automatic PASS/FAIL Checks

`timescale 1ns / 1ps

module tb_alu_refined;

    reg  [7:0]  InputA;
    reg  [7:0]  InputB;
    reg  [2:0]  OpCode;
    wire [15:0] OutALU;
    wire        COut;

    integer error_count = 0;
    integer test_case_num = 0;

    // Instantiate UUT
    alu_8bit uut (
        .InputA(InputA),
        .InputB(InputB),
        .OpCode(OpCode),
        .OutALU(OutALU),
        .COut(COut)
    );

    // Verification Check Task
    task check_result(
        input [15:0] expected_out,
        input        expected_cout,
        input [128*8:1] op_name
    );
        begin
            test_case_num = test_case_num + 1;
            #1; // Allow signal settling time
            if ((OutALU !== expected_out) || (COut !== expected_cout)) begin
                $display("[FAIL] Test %0d (%0s) | A=%d B=%d Op=%b | Expected: Out=%d COut=%b | Got: Out=%d COut=%b",
                         test_case_num, op_name, InputA, InputB, OpCode, expected_out, expected_cout, OutALU, COut);
                error_count = error_count + 1;
            end else begin
                $display("[PASS] Test %0d (%0s) | A=%d B=%d Op=%b | Out=%d COut=%b",
                         test_case_num, op_name, InputA, InputB, OpCode, OutALU, COut);
            end
        end
    endtask

    initial begin
        $display("=================================================");
        $display("      STARTING ALU VERIFICATION SUITE           ");
        $display("=================================================");

        // --- MANDATORY TEST CASE 1: A = 45, B = 38 ---
        InputA = 45; InputB = 38;
        
        OpCode = 3'b000; #9; check_result(16'd83, 1'b0, "ADD");
        OpCode = 3'b001; #9; check_result(16'd7,  1'b0, "SUB");
        OpCode = 3'b110; #9; check_result(16'd47, 1'b0, "OR");
        OpCode = 3'b111; #9; check_result(16'd11, 1'b0, "XOR");

        // --- MANDATORY TEST CASE 2: A = 49, B = 10 ---
        InputA = 49; InputB = 10;
        
        OpCode = 3'b010; #9; check_result(16'd490, 1'b0, "MUL");
        OpCode = 3'b011; #9; check_result(16'd196, 1'b0, "SHL"); // 49 << (10 & 7 = 2) = 196
        OpCode = 3'b100; #9; check_result(16'd12,  1'b0, "SHR"); // 49 >> 2 = 12
        OpCode = 3'b101; #9; check_result(16'd0,   1'b0, "AND"); // 49 & 10 = 0

        // --- ADDITIONAL EDGE CASES ---
        $display("\n--- Running Additional Edge Cases ---");
        
        // Overflow Addition
        InputA = 200; InputB = 100; OpCode = 3'b000; #9;
        check_result(16'd44, 1'b1, "ADD Overflow"); // 300 = 256 + 44 -> Out=44, COut=1

        // Maximum Value Multiplication
        InputA = 255; InputB = 255; OpCode = 3'b010; #9;
        check_result(16'd65025, 1'b0, "MUL Max");

        // Final Summary
        $display("=================================================");
        if (error_count == 0)
            $display("   ALL TESTS PASSED SUCCESSFULLY! (%0d tests)", test_case_num);
        else
            $display("   TEST SUITE FAILED WITH %0d ERRORS!", error_count);
        $display("=================================================");
        $finish;
    end

endmodule