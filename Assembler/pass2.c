//
//  pass2.c
//  Assembler
//
//  Created by Giancarlo Anemone and Caitlin Braun on 5/7/13.
//
/*
 * This function reads the lines in an assembly source file and looks
 * for mips assembly code statements.  It takes those statements and converts 
 * them into binary, and prints out the corresponding machine instruction 
 * binary representations.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>            /* might be memory.h on some machines */
#include <math.h>

#include "pass2.h"
#include "LabelTable.h"
#include "getToken.h"

/*LabelTable pass2 reads in each instruction from the file.  It then finds the corresponding binary representation of the instruction.  
 *Parameters include the pointer to the file name and a LabelTable
 *Pass2 returns a LabelTable
 */
LabelTable pass2 (char * filename, LabelTable table)
/* returns a copy of the label table that was constructed */
{
    FILE * fp;                              /* file pointer */
    char * tokBegin, * tokEnd;              /* used to step thru inst */
    int    SAME=0, PC = 0, address;         /* the program counter */
    char   inst[BUFSIZ], *rs, *rt, *rd,*shamt,*label;           /* will hold instruction; BUFSIZ
                                    is max size of I/O buffer */
    const char * ERROR1 = "Error: Cannot open file %s.\n";
    char * name, *info[8];
    
    /* create a small label table to begin with */
    if ((fp = fopen (filename, "r")) == NULL)
    {
        (void) fprintf (stderr, ERROR1, filename);
        return table;
    }
    
    /* Continuously read next line of input until EOF is encountered.
     * Check each line to see if it has an instruction
     */
    for (PC = 0; fgets (inst, BUFSIZ, fp); PC += 4)
    {
        /* Get rid of comment at end of line (if any).
         * If the line starts with a comment, move on to next line.
         */
        if ( *inst == '#' ) continue;
        (void) strtok (inst, "#");
        
        /* Skip any leading whitespace. */
        tokBegin = inst;
        getToken (&tokBegin, &tokEnd);
        
        /* tokBegin now points to 1st non-whitespace character
         * in the token and tokEnd points to 1st punctuation mark
         * or whitespace after the end of the token.
         */        
        if ( *(tokEnd) == ':' )
        {
            tokBegin = tokEnd + 1;
            getToken(&tokBegin, &tokEnd);
        }
        *tokEnd = '\0';      /* truncate everything after first instruction */
        name=tokBegin;
        (void)getNameInfo(name, &info);
        //gets registers rd, rs, rt for majority of R type instructions
        if(info[7]==(char*)'0')
        {
            getRegisters(&rs, &rt, &rd, NULL, NULL, tokBegin, tokEnd);
            shamt = "0";
        }
        //gets registers rd, rs, shamt for sll and srl operations
        else if(info[7]==(char*)'1')
        {
            getRegisters(NULL,&rt, &rd, &shamt, NULL, tokBegin, tokEnd);
            rs = "$zero";
        }
        //gets reister rd for jr operation
        else if(info[7]==(char*)'2')
        {
            getRegisters(&rs,NULL,NULL,NULL,NULL, tokBegin, tokEnd);
            if(strcmp(rs, "$ra") != SAME)
            {
                (void)fprintf(stderr, "The jr command must be followed by the $ra register \n");
            }
            rt = "$zero";
            rd = "$zero";
            //address = findLabel(&table, rd)/4;
        }
        //gets registers rs, rd, shamt for andi, ori, addi, addiu, stli, stliu operations
        else if(info[7]==(char*)'3')
        {
            getRegisters(&rs, NULL, &rd, &shamt, NULL, tokBegin, tokEnd);
        }
        //gets registers rd, shamt, rs lw and sw operation
        else if(info[7]==(char*)'4')
        {
            getRegisters(&shamt, NULL, &rd, &rs, NULL, tokBegin, tokEnd);
        }
        //gets registers rd, shamt for lui operation
        else if(info[7]==(char*)'5')
        {
            getRegisters(NULL, NULL, &rd, &shamt, NULL, tokBegin, tokEnd);
            rs="$zero";
        }
        //Gets registers rs, rt, address for beq, bne operation
        else if(info[7]==(char*)'6')
        {
            getRegisters(&rs, &rt, NULL, NULL, &label, tokBegin, tokEnd);
            address=findLabel(&table, label)/4;
        }
        //gets registers for j type instruction code: op and address
        else if(info[7]==(char*)'7')
        {
            getRegisters(NULL, NULL, NULL, NULL, &label, tokBegin, tokEnd);
            address=findLabel(&table,label)/4;
        }
        //Print's for an I type instruction
        if(strcmp(info[6],"I")==SAME)
        {
            // Get integer value of shamt
            int shamtInt = atoi(shamt);
            // print opcode
            (void)fprintf(stdout, "%s", info[0]);
            (void)fprintf(stdout, "%s", getRegisterCode(rs));
            (void)fprintf(stdout, "%s", getRegisterCode(rd));
            if(info[7] == (char*)'6')
                (void)fprintf(stdout, "%s", getBinary(address, 15));
            else
                (void)fprintf(stdout, "%s", getBinary(shamtInt, 15));
            (void)fprintf(stdout,"\n");
            
        }
        //Prints for an R type instruction
        if(strcmp(info[6],"R")==SAME)
        {
            int shamtInt = atoi(shamt);
            (void)fprintf(stdout, "%s", info[0]);
            (void)fprintf(stdout, "%s", getRegisterCode(rs));
            (void)fprintf(stdout, "%s", getRegisterCode(rt));
            (void)fprintf(stdout, "%s", getRegisterCode(rd));
            (void)fprintf(stdout, "%s", getBinary(shamtInt, 4));
            (void)fprintf(stdout, "%s", info[5]);
            (void)fprintf(stdout,"\n");
        }
        //Prints for a J type instruction
        if(strcmp(info[6],"J")==SAME)
        {
            (void)fprintf(stdout, "%s", info[0]);
            (void)fprintf(stdout, "%s", getBinary(address, 25));
            (void)fprintf(stdout,"\n");
        }
    }
            /* EOF */
    (void) fclose (fp);
    return table;
}

/* getNetToken finds the next token in the line using delimeters of ',', '(', ')', ':', or '\0'
 * Parameters include the pointer to the register, the token beginning and the token end
 */
int getNextToken(char **reg,char **tokBegin,char **tokEnd)
{
    //sets beginning token to the end of the previous token
    **tokBegin = **tokEnd + 1;
    //finds the next token starting at the new token beginning
    getToken(tokBegin, tokEnd);
    tokEnd = '\0';      /* truncate everything after first instruction */
    //sets register pointer to the beginning token
    *reg = *tokBegin;
    return 0;
}

/* getNameInfo determines all of the information about the instruction that can be determined by the name of the instruction
 *Paramters include the pointer to the name of the instruction and the pointer to the array containing the information for each instruction
 */
int getNameInfo(char * name, char ** info)
{
    int SAME=0;
    //an array containing every mips instruction used in the program
    static char * nameArray[]={
        "add",
        "addu",
        "sub",
        "subu",
        "and",
        "or",
        "nor",
        "sll",
        "srl",
        "slt",
        "sltu",
        "jr",
        "addi",
        "addiu",
        "andi",
        "ori",
        "lw",
        "sw",
        "lui",
        "beq",
        "bne",
        "slti",
        "sltiu",
        "j",
        "jal",
    };
    //an array containing the corresponding mips op code
    static char * opArray[]={
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "001000",
        "001001",
        "001100",
        "001101",
        "100011",
        "101011",
        "001111",
        "000100",
        "000101",
        "001010",
        "001011",
        "000010",
        "000011",
    };
    //an array containing the corresponding mips function code
    static char* funcNameArray[]={
        "100000",
        "100001",
        "100010",//34
        "100011",
        "100100",
        "100101",
        "100111",
        "000000",//0
        "000010",
        "101010",
        "101011",
        "001000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
        "000000",
    
    };
    //array containing the corresponding MIPS instruction type
    static char* formatArray[]={
        "R",
        "R",
        "R",
        "R",
        "R",
        "R",
        "R",
        "R",
        "R",
        "R",
        "R",
        "R",
        "I",
        "I",
        "I",
        "I",
        "I",
        "I",
        "I",
        "I",
        "I",
        "I",
        "I",
        "J",
        "J",
    };
    //goes through every possible instruction and determines the information obtained from that instruction
    for(int j=0; j<25;j++)
    {
        //checks that the name has been found in the name array 
        if(strcmp(name,nameArray[j])==SAME)
        {
            //sets opCode, function code and format 
            info[0]=opArray[j];
            info[5]=funcNameArray[j];
            info[6]=formatArray[j];
            
            //determines how the insructions will be read in
            if(j<12)
            {
                info[4] = "00000";
                if(j<=6||j==9||j==10)
                {
                    info[7]=(char*)'0';//registers read in as add is read in
                }
                else if(j==7||j==8)
                {
                    info[7]=(char*)'1';//registers read in for sll and srl
                }
                else
                {
                    info[7]=(char*)'2';//reg read in for jr
                }
                
            }
            
            else if(j<16||j==21||j==22)
            {
                info[7]=(char*)'3';//registers read in for addi style
            }
            else if(j<18)
            {
                info[7]=(char*)'4';//registers read in for lw and sw
            }
            else if(j<19)
            {
                info[7]=(char*)'5';//registers read in for lui
            }
            else if(j<21)
            {
                info[7]=(char*)'6';//registers read in for beq and bne
            }
            else
            {
                info[7]=(char*)'7';//registers read in for j and jal
            }
            return 1;
        }
    }
    (void) fprintf (stderr, "Error: %s is not a valid operation name\n", name);
    return 1;
}

/* getRegisterCode determines the binary representation of each register 
 *paramaters include the pointer to the register name 
 */
char* getRegisterCode(char * regName)
{
    /* a static (persistent) array of the mnemonic names,
     **    each of which is a string (char *).
     */
    int SAME=0;
    //(void)fprintf(stdout, "Reg Name: %s \n", regName);
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
    //a static array of the corresponding binary representation for each register
    static char* binRegArray[]={
        "00000",
        "00001",
        "00010", "00011",
        "00100", "00101", "00110", "00111",
        "01000", "01001", "01010", "01011", "01100", "01101", "01110", "01111",
        "10000", "10001", "10010", "10011", "10100", "10101", "10110", "10111",
        "11000", "11001",
        "11010", "11011",
        "11100",
        "11101",
        "11110",
        "11111",
    };
    //when the regArray is the same is the regName, the binary value of the register is returned
    for(int j=0; j<32;j++)
    {
        if(strcmp(regName,regArray[j])==SAME)
        {
            return binRegArray[j];
            return 0;
        }
    }
    (void) fprintf (stderr, "Error: %s is not a valid register name\n", regName);
    return "00000";
}


/*getRegisters assigns the source register, the shift amount, the destination register, and the address to the instruction
 *Paramaters include two source registers, destination register, shift amount, address, token begin and token end.  When one of the parameters is null the register will not be assigned as it is not needed for the instruction
 */
int getRegisters(char** rs, char** rt, char** rd, char** shamt, char** address, char* tokBegin, char* tokEnd)
{
    //when rd is not null the destination register is set
    if(rd != NULL)
    {
        tokBegin = tokEnd + 1;
        getToken(&tokBegin, &tokEnd);
        *tokEnd = '\0';
        *rd = tokBegin;
    }
    //when rs is not null the first source register is set
    if(rs != NULL)
    {
        tokBegin = tokEnd + 1;
        getToken(&tokBegin, &tokEnd);
        *tokEnd = '\0';
        *rs = tokBegin;
    }
    //when rt is not null the second source register is set
    if(rt != NULL)
    {
        tokBegin = tokEnd + 1;
        getToken(&tokBegin, &tokEnd);
        *tokEnd = '\0';
        *rt = tokBegin;
    }
    
    //when shift amount is not null the shift amount is set
    if(shamt != NULL)
    {
        tokBegin = tokEnd + 1;
        getToken(&tokBegin, &tokEnd);
        *tokEnd = '\0';
        *shamt = tokBegin;
    }
    //when the address is not null the shift amount is set
    if(address != NULL)
    {
        tokBegin = tokEnd + 1;
        getToken(&tokBegin, &tokEnd);
        *tokEnd = '\0';
        *address = tokBegin;
    }
    return 0;
}


/*getBinary function determines the unsigned binary value of a particular decimal number for a given number of bits
 *parameters include the decimal nubmer and the number of bits the binary result will be expressed in
 *returns a pointer to the string containing the binary representation
 */
char* getBinary(int dec, int range)
{
    int k = 0;
    char* binary[15];
    binary[0] = '\0';
    while(k <= range)
    {
        //when the decimal value is greater than 2 raised to the (range-k) power a 1 is placed on the binary string and the remainder is found
        if(dec >= (pow(2,range-k)))
        {
            (void)strcat(binary, "1");
            dec -= pow(2,(range-k));
        }
        //a zero is placed on the binary string
        else
            (void)strcat(binary, "0");
        k++;
    }
    //binary string is returned
    return binary;
}