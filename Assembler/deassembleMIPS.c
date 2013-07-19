//
//  deassembleMIPS.c
//  PP2
//
//  Created by Giancarlo Anemone and Caitlin Braun on 4/17/13.
//

#include <stdio.h>
#include <string.h>
#include "deassembleMIPS.h"
/*
 Function to get the decimal representation of the op code.
 inputs - char string[], the binary mips instruction
 returns - int, the decimal representation of the op code
 */
int getDecimalOPCode(char string[])
{
    return binToDec(string, 0, 6);
}
/*
 Function to get the character value of the instruction format for a given line of MIPS code
 inputs - char string[], a line of binary MIPS code
 returns - char*, the character value of the instruction format (either I, J, or R
 */
char* getInstructionFormat(char string[])
{
    int opCode = binToDec(string, 0, 6);
    char* iFormat = "I";
    if(opCode == 0)
        iFormat = "R";
    else if(opCode == 2 || opCode == 3)
        iFormat = "J";
    return iFormat;
}

/*
 * This function looks up and returns the mnemonic name for a given
 * register number.
 *
 *   Pre-condition: 0 <= registerNumber < 32
 *   Returns: returns the mnemonic name associated with registerNumber
 *
 * How to call this function:
 *    You do not need to allocate space for the mnemonic name that this
 *    function returns; it will return a pointer to a name (a string)
 *    that is statically scoped (i.e., does not disappear as soon as the
 *    function returns).  For example:
 *	  int rNbr1, rNbr2, rNbr3;	   // register numbers between 0 and 31
 *	  char *rName1, *rName2, *rName3;  // will point to names, e.g., "$t0"
 *	  ...				   // set register numbers
 *	  rName1 = getRegName (rNbr1);
 *	  rName2 = getRegName (rNbr2);
 *	  rName3 = getRegName (rNbr3);
 *	  printf ("add %s, %s, %s\n", rName1, rName2, rName3);
 *    If rNbr1 = 8, rNbr2 = 9, rNbr3 = 16, this will print:
 *		add $t0, $t1, $s0
 *
 * Author: Alyce Brady and Garrett Olson
 * Date:   2/10/99
 *
 * WARNING: This function is incomplete !!!!!!
 */

/*
Function to get the name for the register.
Input - int, the number of the register.
Return- char*, mnemonic name of register.
 
*/
char * getRegisterName (int regNbr)
/* returns mnemonic name for given register */
{
	/* Create a static (persistent) array of the mnemonic names,
     **    each of which is a string (char *).
     */
	static char * regArray[] = {
		"$zero",
		"$at",
		"$v0","$v1",
		"$a0","$a1","$a2","$a3",
		"$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
		"$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7",
		"$t8","$t9",
		"$k0","$k1",
		"$gp",
		"$sp",
		"$fp",
		"$ra",
	};    
    // returns the mnemonic name for the register if it is not null
    if(regArray[regNbr]!=NULL)
        return regArray[regNbr];
    else
        return "NV"; // returns NV for No Value if the register does not exist
}
/*
 Function to get the Function name for an R Instruction format instruction.
 Input - int, function code
 Return - char*, mnemonic name for the function 
 */
char * getRFuncNames(int function)
{
    /* Create a static (persistent) array of the mnemonic names,
     **    each of which is a string (char *).
     */
    static char* RFuncNames[44];
    RFuncNames[0] = "sll";
    RFuncNames[2] = "srl";
    RFuncNames[8] = "jr";
    RFuncNames[32] = "add";
    RFuncNames[33] = "addu";
    RFuncNames[34] = "sub";
    RFuncNames[35] = "subu";
    RFuncNames[36] = "and";
    RFuncNames[37] = "or";
    RFuncNames[39] = "nor";
    RFuncNames[42] = "slt";
    RFuncNames[43] = "sltu";
    // returns the mnemonic name for the function if it is not null
    if(RFuncNames[function]!=NULL)
        return RFuncNames[function];
    else
        return "NV";
}
/*
 Function to get the Function name for an I Instruction format instruction.
 Input - int, function code
 Return - char*, mnemonic name for the function
 */
char * getINames(int op)
{
    /* Create a static (persistent) array of the mnemonic names,
     **    each of which is a string (char *).
     */
    static char* INames[44];
    INames[2] = "j"; 
    INames[3] ="jal";
    INames[4] = "beq";
    INames[5] = "bne";
    INames[8] = "addi";
    INames[9] = "addiu";
    INames[10] = "slti";
    INames[11] = "sltiu";
    INames[12] = "andi";
    INames[13] = "ori";
    INames[15] = "lui";
    INames[35] = "lw";
    INames[43] = "sw";
    // returns the mnemonic name for the function if it is not null
    if(INames[op]!=NULL)
        return INames[op];
    else
        return "NV";
}
/*
 Function to get the register number from a binary command.
 inputs - char string[], the full binary instruction
          int inputNumber, 0 for rs, 1 for rt, 2 for rd.
 Returns - int, number of the register.
 */
int getRegisterNumber(char string[], int inputNumber)
{
    return binToDec(string, 0 + 6 + 5*(inputNumber), 5 + 6 + 5*(inputNumber));
}
/*
 Function to print the assembly language instruction from a binary command
 Inputs - char string[], the 32 bit binary instruction
          int lineNum - current line number in program.
 Returns - int 1 if success, 2 if failure.
 */
int PrintInstruction(char string[], int lineNum)
{
    char * iFormat = getInstructionFormat(string); // Instruction Format (R, I, J)
    char * rs;                                     // rs register
    char * rt;                                     // rt register
    char * rd;                                     // rd register
    char * opName;                                 // operation name
    int address,op,func,shamt;                     // ints for address, opcode, function, and shamt
    // if the command is an I Format instruction...
    if(strcmp(iFormat, "I")==0)
    {
        // get the op code
        op=getDecimalOPCode(string);
        // get the rs and rt registers
        rs= getRegisterName(getRegisterNumber(string, 0));
        rt=getRegisterName(getRegisterNumber(string, 1));
        // get the address
        address=binToDec(string,16,32);
        // get the operation name 
        opName=getINames(op);
        // if the registers are invalid, print an error
        if(strcmp(rt,"NV")==0||strcmp(rs,"NV")==0)
        {
            (void) fprintf(stderr, "Error Line %d: Not a valid register \n",lineNum);
            return 0;
        }
        // if the operation name is invalid, print an error
        if(strcmp(opName, "NV")==0)
        {
            (void) fprintf(stderr, "Error Line %d: Not a valid operator name\n", lineNum);
            return 0;
        }
        else
        {
            // check opcode <= 5
            if(op<=5)
            {
                // special print statement for beq and bne
                (void) fprintf(stdout, "%s \t %s, %s, %d \n", opName, rs, rt, address*4);
                return 1;
            }
            // check opcode > 5 and < 14
            else
            if(op<14)
            {
                // formatted print statement
                (void) fprintf(stdout, "%s \t %s, %s, %d \n", opName, rt, rs, address);
                return 1;
            }
            else
            {
                // check opcode >= 14 < 16
                if(op<16)
                {
                    // check valid instruction
                    if(binToDec(string,6,11)==0)
                    {
                        // formatted print statement
                        (void) fprintf(stdout, "%s \t %s, %d \n", opName, rt, address);
                        return 1;
                    }
                    else
                    {
                        // invalid instruction
                        (void) fprintf(stderr, "Error Line %d: Not a valid MIPS Instruction Code\n",lineNum);
                        return 0;
                    }
                    
                }
                // opcode > 16
                else
                {
                    // formatted print statement
                    (void) fprintf(stdout, "%s \t %s, %d(%s) \n", opName, rt, address, rs);
                    return 1;
                }
            }
        }
        return 0;
    }
    // if the command is an I Format instruction...
    else if(strcmp(iFormat, "J")==0)
    {
        // get the op code
        op=binToDec(string,0,6);
        // get the address
        address=binToDec(string, 6,32);
        // special code for j instruction
        if(op==2)
        {
            opName="j";
            // formatted print statement for j instruction
            (void) fprintf(stdout, "%s \t %d \n", opName, 4*address);
            return 1;
        }
        else
        {
            // special code for jal instruction
            if(op==3)
            {
                opName="jal";
                // formatted print statement for jal instruction
                (void) fprintf(stdout, "%s \t %d \n", opName, 4*address);
                return 1;
            }
            else
            {
                // error statement for non valid operator
                (void) fprintf(stderr, "Error Line %d: Not a valid operator name\n",lineNum);
                return 0;
            }
            
        }
    }
    // if the command is an R Format instruction...
    else if(strcmp(iFormat, "R")==0)
    {
        // the opcode must be zero
        op=0;
        // get the registers (rd, rs, rt)
        rt=getRegisterName(getRegisterNumber(string, 1));
        rd=getRegisterName(getRegisterNumber(string, 2));
        rs= getRegisterName(getRegisterNumber(string, 0));
        // get the decimal shamt code
        shamt = getShamt(string);
        // get the decimal func code
        func = getFunctionCode(string);
        // get the operation name
        opName=getRFuncNames(func);
        // if the operation name is "NV" for no value...
        if(strcmp(opName, "NV")==0)
        {
            // formatted error statement for invalid operator 
            (void) fprintf(stderr, "Error Line %d: Not a valid operator name\n",lineNum);
            return 0;
        }
        else
        {
            // if the function code is < 3...
            if(func <=3)
            {
                // make sure all the registers have valid names
                if(strcmp(rt,"NV")==0||strcmp(rd,"NV")==0||strcmp(rs,"NV")==0)
                {
                    // formatted error statement if the registers have an invalid name
                    (void) fprintf(stderr, "Error Line %d: Not a valid register \n",lineNum);
                    return 0;
                }
                // check that the binary instruction is valid
                if(binToDec(string,6,11)==0)
                {
                    // formatted print statement for the instruction
                    (void) fprintf(stdout, "%s \t %s, %s, %d\n", opName, rd, rt, shamt);
                    return 1;
                }
                else
                {
                    // formatted error statement for an invalid instruction
                    (void) fprintf(stderr, "Error Line %d: Not a valid MIPS Instruction Code\n",lineNum);
                    return 0;
                }
            }
            else
            {
                // if the function code is < 9
                if(func<=9)
                {
                    // check that it is a valid instruction
                    if(binToDec(string,11,26)==0)
                    {
                        // formatted print statement for the instruction
                        (void) fprintf(stdout, "%s \t %s \n", opName, rs);
                        return 1;
                    }
                    else
                    {
                        // formatted error statement for an invalid mips instruction
                        (void) fprintf(stderr, "Error Line %d: Not a valid MIPS Instruction Code\n",lineNum);
                        return 0;
                    }
                }
                else
                {
                    // if the instruction is valid
                    if(binToDec(string,21,26)==0)
                    {
                        // formatted print statement for the instruction
                        (void) fprintf(stdout, "%s \t %s, %s, %s \n", opName, rd, rs, rt);
                        return 1;
                    }
                    else
                    {
                        // formatted error statement for an invalid mips instruction
                        (void) fprintf(stderr, "Error Line %d: Not a valid MIPS Instruction Code\n",lineNum);
                        return 0;
                    }
                }
            }
        }
    }
    return 0;
}
/*
Function to get the shamt code for a given binary instruction
inputs - char string[], the binary instruction
Returns- int, the decimal value of the shamt code
 */
int getShamt(char string[])
{
    return binToDec(string, 21, 26);
}
/*
Function to get the func code.
inputs - char string[], the binary instruction
Returns- int, the decimal value of the function code
 */
int getFunctionCode(char string[])
{
    return binToDec(string, 26, 32);
}






