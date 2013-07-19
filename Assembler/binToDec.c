/*
 * binToDec
 *
 * This function converts the binary digits from string[begin] to
 * string[end] to a decimal integer.
 *
 * int binToDec(char string[], int begin, int end)
 *   Pre-condition:  begin and end are valid indices in array string;
 *   		     every character from string[begin] to string[end]
 *		     (including those two) has a character value '0' or '1'
 *   Returns: the decimal equivalent of the binary number represented by
 *	      the characters in string[begin] - string[end].
 *            (If begin > end, this function arbitrarily returns 0.)
 *
 * For example, assume A is a character array that contains the
 *    following characters:
 *            '1'  '0'  '1'  '1'  '0'  '1'  '0'  '0'  '1'
 *    The call binToDec(A, 2, 5) converts the binary number 1101
 *    (represented by the substring A[2] - A[5]) to the decimal integer
 *    13 and returns it.
 *
 * Implementation:
 * function binToDec takes a binary expression and converts it to
 * a decimal number.  The function loops through an array of characters
 * representing a binary expression.  If the current value = 1, a decimal
 * counter starting at 0 is incremented to 2*(itself)+1.  If the current
 * value is 0, the decimal counter is incremented to 2*(itself).
 *
 * Author: Giancarlo Anemone, Caitlin Braun
 *	Will G.
 *
 * Creation Date:  4/11/2013
 *
 */

int binToDec(char string[], int begin, int end)
{
    char one='1';
    char zero='0';
	int i;
    int decimal = 0;
    // loops through the char array
	for(i=begin; i<end; i++)
    {
        // adds appropriate values to the decimal value
        decimal= 2 * decimal + ((string[i] == one) ? 1 :0);
    }
    
	return decimal;
}