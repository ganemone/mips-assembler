/*
 * This function reads the lines in an assembly source file and looks
 * for labeled statements.  It builds a table of labels and addresses.
 * It returns a copy of the table it created.  If an error occurs, the
 * function prints an error message and returns the table as it exists
 * at that point (possibly empty).
 *
 * Author: Alyce Brady
 * Date:   2/16/99
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>            /* might be memory.h on some machines */

#include "LabelTable.h"
#include "getToken.h"

LabelTable pass1 (char * filename)
/* returns a copy of the label table that was constructed */
{
    LabelTable table;              /* the table of labels & addresses */
    FILE * fp;                     /* file pointer */
    char * tokBegin, * tokEnd;     /* used to step thru inst */
    int    PC = 0;                 /* the program counter */
    char   inst[BUFSIZ];           /* will hold instruction; BUFSIZ
                                    is max size of I/O buffer */
    const char * ERROR1 = "Error: Cannot open file %s.\n";
   
    /* create a small label table to begin with */
    tableInit (&table);
    if ( tableResize (&table, 10) == 0)
    {
        /* error message already printed */
        return table;
    }
    /* open the file for reading */

    if ((fp = fopen (filename, "r")) == NULL)
    {
        (void) fprintf (stderr, ERROR1, filename);
        return table;
    }
    
    /* Continuously read next line of input until EOF is encountered.
     * Check each line to see if it has a label; if it does, add it
     * to the label table.
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
            /* Line has a label! */
            *tokEnd = '\0';      /* truncate everything after label */
            /* Add label to table */
            if (addLabel (&table, tokBegin, PC) == 0)
            {
                /* error message already printed */
                (void) fclose (fp);
                return table;
            }
        }
        tokBegin = tokEnd+1;
        getToken(&tokBegin, &tokEnd);
        *tokEnd = '\0';
    }
    
    /* EOF */
    (void) fclose (fp);
    return table;
}
