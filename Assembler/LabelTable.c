/*
 * Label Table: functions to access and manipulate a label table
 *
 * This file provides the definitions of a set of functions for
 * creating, maintaining, and using a table of assembly language
 * instruction labels and the memory addresses associated with them.
 *
 * Author: Alyce Brady (modified by Giancarlo Anemone and Caitlin Braun)
 *
 * Creation Date:   2/16/99
 *   Modified:  4/11/2013   Implemented the tableInit, addLabel, 
 * findLabel, and printLabels methods.
 *   
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>             /* might be memory.h on some machines */

#include "LabelTable.h"

static const char * ERROR1 = "Error: cannot allocate space in memory.\n";
static const char * ERROR2 = "Error: a duplicate label was found.\n";

const int SAME = 0;             /* useful for making strcmp readable */
/* e.g., if (strcmp (str1, str2) == SAME) */

void tableInit (LabelTable * table)
/* Postcondition: table is initialized to indicate that there
 *       are no label entries in it.
 */
{
    // initialize table to have zero entries
    table->nbrLabels = 0;
    table->capacity = 0;
    table->entries = 0;
}

int tableResize (LabelTable * table, int newSize)
/* Postcondition: table now has the capacity to hold newSize
 *      label entries.  If the new size is smaller than the
 *      old size, the table is truncated after the first
 *      newSize entries.
 * Returns 1 if everything went OK; 0 if memory allocation error or
 *      current table doesn't exist.
 */
{
    char *       ptr;              /* points to any byte in memory */
    LabelEntry * newEntryList;
    int          smaller;
    
    /* make sure that current table exists */
    if ( table == NULL )
        return 0;
    
    /* create a new internal table of the specified size */
    if ((newEntryList = malloc (newSize * sizeof(LabelEntry))) == NULL)
    {
        (void) fprintf (stderr, ERROR1);
        return 0;
    }
    
    if ( table->entries )           /* if there were entries */
    {
        /* move contents of internal table to new internal table */
        smaller = table->nbrLabels < newSize ? table->nbrLabels : newSize;
        (void) memcpy (newEntryList, table->entries,
                       smaller * sizeof(LabelEntry));
        
        /* free the space taken up by the old internal table */
        free (table->entries);
        table->nbrLabels = smaller;
    }
    
    table->entries = newEntryList;
    table->capacity = newSize;
    return 1;
}

int addLabel (LabelTable * table, char * label, int PC)
/* Postcondition: if label was already in table, the table is
 *      unchanged; otherwise a new entry has been added to the
 *      table with the specified label name and instruction address
 *      (memory location) and the table has been resized if necessary.
 * Returns 1 if no fatal errors occurred; 0 if memory allocation error
 *      or table doesn't exist.
 */
{
    char * labelDuplicate;
    //fprintf(stdout, "LABEL ADDRESS: %d \n", PC);
    /* make sure that current table exists */
    if ( table == NULL )
        return 0;
    
    /* Create a dynamically allocated version of label that will persist.
     */
    /*   NOTE: on some machines you may need to make this _strdup !  */
    if ((labelDuplicate = strdup (label)) == NULL)
    {
        (void) fprintf (stderr, ERROR1);
        return 0;           /* fatal error: couldn't allocate memory */
    }
    
    //check that it is not in the table
    if(findLabel(table, label) == -1)
    {
        //checks capacity
        if(table->nbrLabels >= table->capacity)
        {   // resizes table
            tableResize(table, table->capacity+1);
        }
        LabelEntry * newEntry = table->entries + table->nbrLabels; //points the first
        newEntry->label = labelDuplicate;
        newEntry->address = PC;
        ++table->nbrLabels;
    }
    else
    {
        //(void) fprintf(stdout, "The label already exists \n");
    }
    //printLabels(table);
    return 1;               
}

int findLabel (LabelTable * table, char * label)
/* Returns the address associated with the label; -1 if label is
 *      not in the table
 */
{
    int i;
    //create pointer to first entry in table
    LabelEntry *newEntry = table->entries;
    //loop through the labels
    for(i=0;i<table->nbrLabels;i++)
    {
        //check to see if lable at address is equal
        if(strcmp(newEntry->label,label)==0)
        {
            return newEntry->address;
        }
        newEntry++;
    }
    return -1;
    
}

void printLabels (LabelTable * table)
/* Postcondition: all the labels in the table have been printed
 *      to the standard output.
 */
{
    int i;
    LabelEntry *newEntry = table->entries;
    if ( table == NULL )
        (void) printf ("Label Table is a NULL pointer.\n");
    else
    {
        (void) printf ("There are %d labels in the table:\n",
                       table->nbrLabels);
        
        for(i=0; i<table->nbrLabels;i++)
        {
            printf("Label Value: %s, Label Address: %d \n",newEntry->label,newEntry->address);
            newEntry++;
        }
    }
}