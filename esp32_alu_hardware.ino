/*
 * ESP32 8-Bit Hardware ALU - Final DIP Switch & SSD1306 OLED Firmware
 * Board: DOIT ESP32 DEVKIT V1 (30-Pin)
 * Inputs: 18x Active-LOW DIP Switches (GND = 1) + 1x Active-HIGH on D2/B7 (3V3 = 1)
 * Display: SSD1306 128x64 I2C OLED (SDA = GPIO 21, SCL = GPIO 22)
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Hardware Pin Mappings
const int pinA[8]  = {13, 12, 14, 27, 26, 25, 33, 32}; // Switch 1 & 2 (A0..A7)
const int pinB[8]  = {15,  4, 16, 17,  5, 18, 19,  2}; // Switch 3 & 4 (B0..B7)
const int pinOP[3] = {23,  3,  1};                     // Switch 5 (OP0=D23, OP1=RX0, OP2=TX0)

bool oledReady = false;

void setup() {
  // Configure Input A (all Active-LOW)
  for (int i = 0; i < 8; i++) {
    pinMode(pinA[i], INPUT_PULLUP);
  }

  // Configure Input B: B0..B6 Active-LOW, B7 (D2) Active-HIGH using onboard LED pull-down
  for (int i = 0; i < 7; i++) {
    pinMode(pinB[i], INPUT_PULLUP);
  }
  pinMode(pinB[7], INPUT);

  // Configure OpCode (all Active-LOW)
  for (int i = 0; i < 3; i++) {
    pinMode(pinOP[i], INPUT_PULLUP);
  }

  // Initialize I2C on D21 (SDA) and D22 (SCL)
  Wire.begin(21, 22);

  // Initialize SSD1306 OLED (Standard I2C address 0x3C)
  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    oledReady = true;
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(10, 20);
    display.println("8-BIT HARDWARE ALU");
    display.setCursor(28, 36);
    display.println("INITIALIZED");
    display.display();
    delay(1200);
  }
}

void loop() {
  uint8_t valA = 0;
  uint8_t valB = 0;
  uint8_t op   = 0;

  // Read Input A (Active-LOW: Switch ON = 1, Switch OFF = 0)
  for (int i = 0; i < 8; i++) {
    if (digitalRead(pinA[i]) == LOW) {
      valA |= (1 << i);
    }
  }

  // Read Input B (B0..B6 Active-LOW, B7 Active-HIGH)
  for (int i = 0; i < 7; i++) {
    if (digitalRead(pinB[i]) == LOW) {
      valB |= (1 << i);
    }
  }
  if (digitalRead(pinB[7]) == HIGH) {
    valB |= (1 << 7);
  }

  // Read OpCode (Active-LOW: Switch ON = 1, Switch OFF = 0)
  for (int i = 0; i < 3; i++) {
    if (digitalRead(pinOP[i]) == LOW) {
      op |= (1 << i);
    }
  }

  uint16_t outALU = 0;
  bool cOut = false;
  const char* opName = "";

  // 8-Bit ALU Execution Multiplexer matching Verilog RTL
  switch (op) {
    case 0: // 000: Addition
      outALU = valA + valB;
      cOut   = (outALU > 255);
      outALU &= 0xFF;
      opName = "ADD";
      break;

    case 1: // 001: Subtraction
      outALU = (valA - valB) & 0xFF;
      cOut   = (valA < valB);
      opName = "SUB";
      break;

    case 2: // 010: Multiplication
      outALU = valA * valB;
      cOut   = false;
      opName = "MUL";
      break;

    case 3: // 011: Logical Left Shift
      outALU = valA << (valB & 0x07);
      cOut   = false;
      opName = "SHL";
      break;

    case 4: // 100: Logical Right Shift
      outALU = valA >> (valB & 0x07);
      cOut   = false;
      opName = "SHR";
      break;

    case 5: // 101: Bitwise AND
      outALU = valA & valB;
      cOut   = false;
      opName = "AND";
      break;

    case 6: // 110: Bitwise OR
      outALU = valA | valB;
      cOut   = false;
      opName = "OR";
      break;

    case 7: // 111: Bitwise XOR
      outALU = valA ^ valB;
      cOut   = false;
      opName = "XOR";
      break;
  }

  // Render Live Dashboard on OLED
  if (oledReady) {
    display.clearDisplay();

    // Header Bar
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("8-BIT ALU | OP:");
    display.print(op);
    display.print(" [");
    display.print(opName);
    display.print("]");
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    // Input Values
    display.setCursor(0, 15);
    display.print("A: ");
    display.print(valA);
    display.setCursor(66, 15);
    display.print("B: ");
    display.print(valB);

    // Binary Preview of Inputs
    display.setCursor(0, 26);
    for (int b = 7; b >= 0; b--) display.print((valA >> b) & 1);
    display.setCursor(66, 26);
    for (int b = 7; b >= 0; b--) display.print((valB >> b) & 1);

    display.drawLine(0, 37, 128, 37, SSD1306_WHITE);

    // Output Result & Carry Flag
    display.setTextSize(2);
    display.setCursor(0, 44);
    display.print("OUT:");
    display.print(outALU);

    display.setTextSize(1);
    display.setCursor(98, 42);
    display.print("COUT");
    display.setTextSize(2);
    display.setCursor(105, 50);
    display.print(cOut ? "1" : "0");

    display.display();
  }

  delay(150);
}
