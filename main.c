// Luke Henry
// CMPE-220-02

// Description: The main purpose of this code is to build an assembler by
// reading MIPS assembly instructions and then convert them to binary or hex.
// This code translates MIPS assembly instructions into their binary and
// hexadecimal representations using two main functions: binaryToHex and
// translateMIPS. binaryToHex converts binary strings to hexadecimal format,
// while translateMIPS parses instructions to identify whether they belong to
// the R-format or I-format, recognizes specific instructions and register
// names, then constructs their corresponding binary strings. Additionally, a
// restructureLW function is introduced within the main method, specifically
// designed to reformat "lw" or "sw" instructions into a consistent structure
// before processing. This code parses a file containing MIPS assembly code,
// identifies "lw" or "sw" instructions to reformat them, and then proceeds to
// print the translated binary and hexadecimal representations of the MIPS
// instructions along with their respective memory addresses.

// Analysis: This is a simple assembler built to handle the more common examples
// of instructions, and this code runs smoothly and efficiently. It first reads
// the instruction to identify the format of the instruction, and saves time by
// only doing that comparison once per iteration. After it finds the
// instruction, it then looks at the individial tokens and translates them. In a
// perfect world, my code could have been a little more efficient, but I chose
// to focus on the assignment and used a couple nested for loops. When memory
// needs to be allocated, just the right amount is to save time and space. One
// place that I think is not my best is handling the format of lw and sw;
// however, I think that my solution to the issue was very creative and
// efficient. Most of the common instructions follow a similar format, lw and sw
// are different, so with handling them I decided to restructure them to format
// the instruction to conform to the other instructions. The method would see
// "lw $t0, 32($t1)" for example and restructure it to "lw $t1, $t0, 32". It
// might not be the best way to do it but I think it is adequetaly efficient and
// am proud of it.

// The program exhibits good readability and clarity
// due to descriptive function names and well-segmented logic within each
// function. However, the efficiency could be improved in a few areas. The use
// of string concatenation functions like strcat within loops can lead to
// inefficiencies due to the repetitive reallocation and copying of memory,
// impacting performance, especially with larger inputs. Additionally, the
// nested loops used to identify instructions and register names could lead to
// increased time complexity, especially as the number of instructions or
// registers grows. The memory allocation using malloc for restructuring "lw"
// and "sw" instructions might lead to memory fragmentation and potentially
// affect performance when handling a large number of instructions.
// Optimizations like using more efficient data structures for storing
// instruction sets or employing bitwise operations instead of string
// manipulation could enhance the code's efficiency, especially for larger MIPS
// programs.

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *binaryToHex(const char *binaryInstruction) {
  // Convert the binary string to an integer
  unsigned int intValue = strtol(binaryInstruction, NULL, 2);

  // Allocate memory for the hexadecimal representation
  char *hexRepresentation =
      (char *)malloc(11 * sizeof(char)); // Assuming a 32-bit instruction

  // Convert the integer to hexadecimal format
  snprintf(hexRepresentation, 11, "0x%08X", intValue);

  return hexRepresentation;
}

void translateMIPS(char *instruction, int hexAddress) {
  char *token;
  int isR = 0;
  int isI = 0;
  char concatenatedArray[45];
  strcpy(concatenatedArray, "");
  char spacelessArray[45];
  strcpy(spacelessArray, "");

  char *rInstructions[] = {"add", "sub", "and", "or", "slt", "jr"};
  char *rFunctionCodes[] = {"100000", "100010", "100100",
                            "100101", "101010", "001000"};
  char *iInstructions[] = {"addi", "lw", "sw", "beq"};
  char *iOpcodes[] = {"001000", "100011", "101011", "000100"};
  char *registerNames[] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2",
                           "$a3",   "$t0", "$t1", "$t2", "$t3", "$t4", "$t5",
                           "$t6",   "$t7", "$s0", "$s1", "$s2", "$s3", "$s4",
                           "$s5",   "$s6", "$s7", "$t8", "$t9"};
  char *binaryValues[] = {"00000", "00001", "00010", "00011", "00100", "00101",
                          "00110", "00111", "01000", "01001", "01010", "01011",
                          "01100", "01101", "01110", "01111", "10000", "10001",
                          "10010", "10011", "10100", "10101", "10110", "10111",
                          "11000", "11001"};

  token = strtok(instruction, " ,");
  while (token != NULL) {
    int found = 0;

    // Check if the token matches an R-format instruction
    for (int i = 0; (i < sizeof(rInstructions) / sizeof(rInstructions[0])) &&
                    (isR == 0 && isI == 0);
         i++) {
      if (strcmp(rInstructions[i], token) == 0) {
        strcat(concatenatedArray, rFunctionCodes[i]);
        strcat(spacelessArray, rFunctionCodes[i]);
        isR = 1;
        found = 1;
        break;
      }
    }

    // Check if the token matches an I-format instruction
    for (int i = 0; (i < sizeof(iInstructions) / sizeof(iInstructions[0])) &&
                    (isR == 0 && isI == 0);
         i++) {
      if (strcmp(iInstructions[i], token) == 0) {
        strcat(concatenatedArray, iOpcodes[i]);
        strcat(spacelessArray, iOpcodes[i]);
        isI = 1;
        found = 1;
        break;
      }
    }

    // Check if the token matches a register name
    int isRegister = 0;
    for (int i = 0; i < sizeof(registerNames) / sizeof(registerNames[0]); i++) {
      if (strcmp(registerNames[i], token) == 0) {
        strcat(concatenatedArray, " ");
        strcat(concatenatedArray, binaryValues[i]);
        strcat(spacelessArray, binaryValues[i]);
        isRegister = 1;
        found = 1;
        break;
      }
    }

    // If the token is not a register, check if it's a constant value
    if (!found && !isRegister) {
      int validConst = 1;
      for (int j = 0; j < strlen(token); j++) {
        if (!isdigit(token[j])) {
          validConst = 0;
          break;
        }
      }
      if (validConst) {
        int constant = atoi(token);
        strcat(concatenatedArray, " ");
        if (constant >= 0 && constant <= 65535) {
          for (int i = 15; i >= 0; i--) {
            char binaryChar = ((constant >> i) & 1) + '0';
            strncat(concatenatedArray, &binaryChar, 1);
            strncat(spacelessArray, &binaryChar, 1);
          }
          found = 1;
        } else {
          printf("\nConstant value %d is out of range (must be between 0 and "
                 "65535).\n",
                 constant);
        }
      }
    }

    if (!found) {
      printf("\nInvalid or unrecognized token: %s\n", token);
    }

    token = strtok(NULL, " ,");
  }

  if (!isR && !isI) {
    printf("\nInvalid or unrecognized instruction.\n");
  }
  if (isR) {
    strcat(concatenatedArray, " ");
    for (int i = 0; i < 11; i++) {
      strcat(concatenatedArray, "0");
      strcat(spacelessArray, "0");
    }
    printf("R-format: %s\n", concatenatedArray);
    printf("0x%08X: ", hexAddress);
    printf("%s\n", binaryToHex(spacelessArray));
  } else {
    printf("I-format: %s\n", concatenatedArray);
    printf("0x%08X: ", hexAddress);
    printf("%s\n", binaryToHex(spacelessArray));
  }
}

// Used to restructure a lw or sw instruction to conform to processing style
char *restructure(char *instruction) {
  char op[10], rt[10], imm[10], rs[10];

  sscanf(instruction, "%s %[^,], %[^($](%[^)])", op, rt, imm, rs);

  char *restructured =
      (char *)malloc(strlen(op) + strlen(rs) + strlen(rt) + strlen(imm) + 5);
  sprintf(restructured, "%s %s, %s, %s", op, rs, rt, imm);

  return restructured;
}

int main() {
  int hexAddress = 0x00000000;
  FILE *file = fopen("assembly.asm", "r");
  if (file == NULL) {
    perror("Error opening file");
    return EXIT_FAILURE;
  }

  char line[100];
  while (fgets(line, sizeof(line), file) != NULL) {
    if (line[0] != '#' && line[0] != '\n' && strncmp(line, ".data", 5) != 0 &&
        strncmp(line, ".text", 5) != 0) {
      line[strcspn(line, "\n")] = '\0';

      if (strstr(line, "lw") != NULL || strstr(line, "sw")) {
        translateMIPS(restructure(line), hexAddress);
      } else {
        translateMIPS(line, hexAddress);
      }

      hexAddress += 4;
    }
  }

  fclose(file);
  return EXIT_SUCCESS;
}
