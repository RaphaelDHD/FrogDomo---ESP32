#include "controller.h"

void CodeController::attach(int size) {
  Serial1.begin(9600);  // UART1
  codeSize = size;
  code = new int[size]; // Allocate memory for the dynamic array
}

bool CodeController::readValue() {
  if (Serial1.available() > 0) {
    char receivedChar = Serial1.read();
    switch (receivedChar) {
      case 0xE1:
        enterNumber(1);
        break;
      case 0xE2:
        enterNumber(2);
        break;
      case 0xE3:
        enterNumber(3);
        break;
      case 0xE4:
        enterNumber(4);
        break;
      case 0xE5:
        enterNumber(5);
        break;
      case 0xE6:
        enterNumber(6);
        break;
      case 0xE7:
        enterNumber(7);
        break;
      case 0xE8:
        enterNumber(8);
        break;
      case 0xE9:
        enterNumber(9);
        break;
      case 0xEA:
        break;
      case 0xEB:
        enterNumber(0);
        Serial.println("0");
        break;
      case 0xEC:
        return checkCode();
        break;
      default:
        break;
    }
    return false;
  }
}

void CodeController::enterNumber(int value) {
  for (int i = 0; i < codeSize - 1; i++) {
    code[i] = code[i + 1];
  }
  code[codeSize - 1] = value;
}

bool CodeController::checkCode() {
  if (code[0] == 1 && code[1] == 2 && code[2] == 3 && code[3] == 4) {
    return true;
  }
  return false;
}
