/*
 * This file contains the getToken function, used by both pass1 and
 * pass2 of the assembler project.  The getToken function sets two
 * character pointers so that one points to the beginning of a token
 * and the other points to the first character beyond the end of the
 * token.  Since the function modifies the two character pointers, the
 * ADDRESSES of the pointers must be passed to getToken.
 *
 * See getToken.h for more specific information about how getToken
 * behaves and for an example.
 *
 * Author:          Alyce Brady
 * Creation Date:   2/18/99
 *
 * Modified:  3/17/2000   added colon as a token delimiter so that
 *                        getToken can be used to find labels.
 *
 */

#include <stdio.h>

void getToken (char ** tokBegin, char ** tokEnd)
/* postcondition: if tokBegin or *tokBegin was NULL when getToken was
 *                    called, tokBegin and *tokBegin will be unchanged;
 *                if tokBegin pointed to an empty string or a string
 *                    containing only whitespace, both tokBegin and
 *                    tokEnd will point to the empty string (i.e., a
 *                    string containing only a null byte);
 *                otherwise, *tokBegin will point to the first
 *                    character in the next token and *tokEnd will
 *                    point to the first character AFTER the token
 *                    (possibly a null byte)
 */
{
    /* Make sure that we have a string to step through. */
    if ( tokBegin == NULL || *tokBegin == NULL )
        return;
    
    /* Skip any leading whitespace. */
    while (**tokBegin != '\0' && isspace (**tokBegin))
        (*tokBegin)++;
    if ( **tokBegin == '\0' )
    {
        *tokEnd = *tokBegin;
        return;
    }
    
    /* Find the end of the first token */
    *tokEnd = *tokBegin + 1;
    while (**tokEnd != '\0' && **tokEnd != ',' &&
           **tokEnd != '(' && **tokEnd != ')' && **tokEnd != ':' &&
           !isspace (**tokEnd))
    {
        (*tokEnd)++;
    }
    
    /* (*tokBegin) now points to beginning of token;
     * (*tokEnd) now points to 1st character AFTER token
     */
}

