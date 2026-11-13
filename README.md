# 8-Bit Hardware ALU: RTL to Silicon Prototype

A complete end-to-end design of an 8-bit Arithmetic Logic Unit (ALU). The project includes a software simulation written in Verilog HDL and a physical hardware prototype built on a breadboard.

## 🛠️ Architecture & Tech Stack
- **RTL Design & Simulation:** Verilog HDL, Xilinx Vivado 2018.2
- **Hardware Prototyping:** ESP32 Microcontroller, C++ (Arduino framework)
- **Physical Interface:** 19 DIP Switches (Input control), SSD1306 I2C OLED Display (Output)
- **Operations:** Addition, Subtraction, Multiplication, Bitwise Logical Gates (AND, OR, XOR), and Bit Shifts.

## 💻 Vivado Simulation & Verification
Before hardware deployment, the ALU logic was rigorously simulated using behavioral simulation in Xilinx Vivado.
- Developed a comprehensive testbench (`tb_alu_refined.v`) to verify edge cases.
- Executed 10 automated test cases verifying signed/unsigned operations and custom waveform radices.
- Console logs and waveform analysis confirmed zero logical faults prior to physical prototyping.

## 🔌 Hardware Implementation
The physical prototype translates the Verilog logic into C++ running on an ESP32. 
- Real-time physical inputs are read via a 4-position DIP switch array.
- The microcontroller processes the logic and drives the calculated output directly to an SSD1306 OLED display.
- Serves as a tangible demonstration of instruction set architecture (ISA) fundamentals and low-level bitwise manipulation.
