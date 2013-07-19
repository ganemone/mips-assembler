//
//  deassembleMIPS.h
//  PP2
//
//  Created by Giancarlo Anemone and Caitlin Braun on 4/20/13.
//

#ifndef PP2_deassembleMIPS_h
#define PP2_deassembleMIPS_h
/*
Pre-condition:  begin and end are valid indices in array string;
*   		     every character from string[begin] to string[end]
*		     (including those two) has a character value '0' or '1'
*   Returns: the decimal equivalent of the binary number represented by
*	      the characters in string[begin] - string[end].
*            (If begin > end, this function arbitrarily returns 0.)
*/
int binToDec(char string[], int begin, int end);
/*
 Function to get the register number from a binary command.
 inputs - char string[], the full binary instruction
 int inputNumber, 0 for rs, 1 for rt, 2 for rd.
 Returns - int, number of the register.
 */
int getRegisterNumber(char string[], int inputNumber);
/*
 Pre-condition:  instruction is a null-terminated string
 Post-condition: the characters in "instruction" have not
 been modified
 Returns: 1 if instr contained 32 characters representing binary
 digits ('0' and '1') followed by a null byte
 */
int verifyMIPSInstruction (char string[]);
/*
 Function to get the decimal representation of the op code.
 inputs - char string[], the binary mips instruction
 returns - int, the decimal representation of the op code
 */
int getDecimalOPCode(char string[]);
/*
 Function to print the assembly language instruction from a binary command
 Inputs - char string[], the 32 bit binary instruction
 int lineNum - current line number in program.
 Returns - int 1 if success, 2 if failure.
 */
int PrintInstruction(char string[], int lineNum);
/*
 Function to get the character value of the instruction format for a given line of MIPS code
 inputs - char string[], a line of binary MIPS code
 returns - char*, the character value of the instruction format (either I, J, or R
 */
char* getInstructionFormat(char string[]);
/*
 Function to get the name for the register.
 Input - int, the number of the register.
 Return- char*, mnemonic name of register.
 */
char* getRegisterName (int regNbr);
/*
 Function to get the shamt code for a given binary instruction
 inputs - char string[], the binary instruction
 Returns- int, the decimal value of the shamt code
 */
int getShamt(char string[]);
/*
 Function to get the func code.
 inputs - char string[], the binary instruction
 Returns- int, the decimal value of the function code
 */
int getFunctionCode(char string[]);


#endif
