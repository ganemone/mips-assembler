/*
 * This is a driver to test pass1 and the Label Table functions.
 *
 * Author: Alyce Brady
 * Date: 2/18/99
 * Modified by: Caitlin Braun and Giancarlo Anemone to test pass2 of the assembler.
 */

#include <stdio.h>
#include <string.h> 
#include "LabelTable.h"

LabelTable pass1 (char * filename);
LabelTable pass2 (char * filename, LabelTable table);


int main ()
{
    LabelTable table;
    int i;
    table = pass1 ("testfile.txt");
    (void)pass2("testfile.txt", table);
    //printLabels (&table);
    
    return 0;
}
