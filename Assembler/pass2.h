//
//  pass2.h
//  Assembler
//
//  Created by Giancarlo Anemone and Caitlin Braun on 5/12/13.
//  Copyright (c) 2013 Giancarlo Anemone and Caitlin Braun. All rights reserved.
//

#ifndef Assembler_pass2_h
#define Assembler_pass2_h

/*getBinary function determines the unsigned binary value of a particular decimal number for a given number of bits
 *parameters include the decimal nubmer and the number of bits the binary result will be expressed in
 *returns a pointer to the string containing the binary representation
 */
char* getBinary(int dec, int range);


/* getRegisterCode determines the binary representation of each register
 *paramaters include the pointer to the register name
 */
char* getRegisterCode(char * regName);


/* getNetToken finds the next token in the line using delimeters of ',', '(', ')', ':', or '\0'
 * Parameters include the pointer to the register, the token beginning and the token end
 */
int getNextToken(char **reg,char **tokBegin,char **tokEnd);


/* getNameInfo determines all of the information about the instruction that can be determined by the name of the instruction
 *Paramters include the pointer to the name of the instruction and the pointer to the array containing the information for each instruction
 */
int getNameInfo(char * name, char ** info);


/*getRegisters assigns the source register, the shift amount, the destination register, and the address to the instruction
 *Paramaters include two source registers, destination register, shift amount, address, token begin and token end.  When one of the parameters is null the register will not be assigned as it is not needed for the instruction
 */
int getRegisters(char** rs, char** rt, char** rd, char** shamt, char** address, char* tokBegin, char* tokEnd);


#endif
