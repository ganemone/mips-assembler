/*
 * verifyMIPSInstruction
 *
 * This function verifies that the string provided to it as a paramater
 * contains 32 characters representing binary digits (characters '0' and
 * '1'), followed by a null byte.
 *
 *   int verifyMIPSInstruction (char * instruction);
 *
 *	    Pre-condition:  instruction is a null-terminated string
 *	    Post-condition: the characters in "instruction" have not
 *			    been modified
 *	    Returns: 1 if instr contained 32 characters representing binary
 *			   digits ('0' and '1') followed by a null byte
 *
 * Implementation:
 * This function simply tests to see that a given array of characters
 * contains only either 1's or 0's. It does not check the length of the
 * string.
 *
 * Authors: Giancarlo Anemone, Caitlin Braun
 *
 *
 * Creation Date:  4/11/2013
 *
 */

#include <string.h>

int verifyMIPSInstruction (char * instr)
/*  returns 1 if instr contains 32 characters representing binary
 *  digits ('0' and '1'); 0 otherwise
 */
{
    
    char one='1', zero='0';
    int i;
    
    for(i=0;i<32;i++)
    {
        if(instr[i]!=one && instr[i]!=zero)
        {
            return 0;
        }
    }
    return 1;
}