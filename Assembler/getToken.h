

#ifndef Assembler_getToken_h
#define Assembler_getToken_h


/*
 * getToken
 *
 * This file contains the declaration for the getToken function, used
 * by both pass1 and pass2 of the assembler project.  The getToken
 * function sets two character pointers so that one points to the
 * beginning of a token and the other points to the first character
 * beyond the end of the token.  Since the function modifies the two
 * character pointers, the ADDRESSES of the pointers must be passed to
 * getToken.
 *
 * More specifically, getToken will move the first pointer whose
 * address it is passed (*tokBegin) forward to the first non-whitespace
 * character, if there is one.  If it comes to a null byte before
 * finding a non-whitespace character, then *tokBegin is set to point
 * to the null byte.  If there WAS a non-whitespace character (the
 * beginning of the token), then getToken will set the second pointer
 * (*tokEnd) to point to the first character after the end of the
 * token, i.e., the first comma, colon, left or right parenthesis,
 * space, or null byte after the character pointed to by *tokBegin.
 *
 * void getToken (char ** tokBegin, char ** tokEnd)
 *   postcondition: if tokBegin was NULL when getToken was called,
 *                      it will still be NULL;
 *                  if tokBegin pointed to an empty string or a string
 *                      containing only whitespace, both tokBegin and
 *                      tokEnd will point to the empty string (i.e., a
 *                      string containing only a null byte);
 *                  otherwise, *tokBegin will point to the first
 *                      character in the next token and *tokEnd will
 *                      point to the first character AFTER the token
 *                      (possibly a null byte)
 *
 * EXAMPLE:
 *
 * For example, consider a function that contains the following string
 * and character pointer variables, and calls getToken as specified.
 *      char aString[] = "   abc: add $t0, $at, $zero   "
 *      char * begin = aString;     // begin points to first char in aString
 *      char * end;                 // end doesn't point to anything yet
 *      getToken (&begin, &end);
 * After the call to getToken, begin will point to the first a in the
 * string and end will point to the colon.
 *                  "   abc:  add $t0, $at, $zero   "
 *                      ^  ^
 *                      |  |
 *                  begin  end
 *
 * The calling function should check that begin != NULL && *begin != '\0'
 * before assuming that begin points to a valid token.  If the
 * calling function wishes to treat the token as a string, it should
 * change *end to a null byte.  If the function needs to know whether
 * this was the last token in the string (indicated by the fact that
 * *end is a null byte), the function must note this first because that
 * information will be lost if *end is deliberately set to be a null byte.
 *      isLastToken = ( *end == '\0' );           // isLastToken is an int
 *      if ( begin != NULL && *begin != '\0' )
 *           *end = '\0';       // begin now points to the string "add"
 *
 * After processing the first token, the function might then move
 * begin to point to the character after end and call getToken again.
 * (It should only do this if *end was not a null byte after the last
 * call to getToken.)
 *      if ( isLastToken )
 *          // no more tokens in string; return or get next string
 *      else
 *      {
 *          begin = end + 1;     // begin points to first char after *end
 *          getToken (&begin, &end);
 *          ...
 *      }
 * After this call to getToken, begin will point to the a in add, and
 * end will point to the space after add.
 *                  "   abc:  add $t0, $at, $zero   "
 *                            ^  ^
 *                            |  |
 *                        begin  end
 *
 * If getToken is called when begin points to one of the spaces after
 * the token $zero, then *begin will be the null byte when getToken
 * returns.
 *
 * Author:          Alyce Brady
 * Creation Date:   2/18/99
 *
 * Modified:  3/17/2000   added colon as a token delimiter so that
 *                        getToken can be used to find labels.
 *
 */

void getToken (char ** tokBegin, char ** tokEnd);

#endif
