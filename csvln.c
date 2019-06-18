
/*
<https://github.com/rafagafe/csvln>
     
  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2019 Rafa Garcia <rafagarcia77@gmail.com>.
  Permission is hereby  granted, free of charge, to any  person obtaining a copy
  of this software and associated  documentation files (the "Software"), to deal
  in the Software  without restriction, including without  limitation the rights
  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
    
*/

#include "csvln.h"
#include <ctype.h>
#include <stdio.h>

/** Remove the blanks from the end. */
static char* rmblk( char* begin, char* end, int sp ) {
    while( begin < end && sp != *--end && isspace ( *end ) );
    end[1] = '\0';
    return begin;
}

/**  Search the next non-space or null character.
  * @param p Pointer to first character.
  * @return  Pointer to the found character. */
static char* skipspace( char* p, int sp ) {
    for( ; '\0' != *p && sp != *p && isspace( *p ); ++p );
    return p;
}

/** Get escape char code code  */
static int getesc( int ch ) {
    static struct { char ch; char code; } const lut [] = {
        { '\"', '\"' }, { '\\', '\\' }, { '/',  '/'  }, { 'b',  '\b' },
        { 'f',  '\f' }, { 'n',  '\n' }, { 'r',  '\r' }, { 't',  '\t' },
        { 'a',  '\a' }, { 'e', '\033' }
    };
    for( int i = 0; i < sizeof lut / sizeof *lut; ++i )
        if ( lut[i].ch == ch )
            return lut[i].code;
    return '\0';
}

/** Parse an argument that is enclosed in quoted strings
  * @param str First character.
  * @return Last character.*/
static char* parseval( char* str ) {
    char* head = str;
    char* tail = str;
    for( ; (unsigned)' ' <= *head && '\"' != *head; ++head, ++tail ) {
        if ( '\\' != *head )
            *tail = *head;
        else {
            int const esc = getesc( *++head );
            if ( '\0' != esc )
                *tail = esc;
            else {
                *tail = '\\';
                *++tail = *head;
            }
        }
    }
    head += '\"' == *head;
    *tail = '\0';
    return head;
}

int csvln( char* dest[], int max, char* ln, int sp ) {
    for( int i = 0; i < max; ++i ) {
        dest[i] = ln = skipspace( ln, sp );
        if( '\"' == *ln ) {
            dest[i] = ++ln;
            ln = parseval( ln );            
            if ( '\r' == *ln || '\n' == *ln || '\0' == *ln ) {
                *ln = '\0';             
                return i + 1;                 
            }
            ln = skipspace( ln, sp );
            if( sp != *ln )           
                return -1;
        }
        else while( sp != *ln ) {
            if ( '\r' == *ln || '\n' == *ln || '\0' == *ln ) {
                *ln = '\0';
                dest[i] = rmblk( dest[i], ln, sp );
                ++ln;                
                return i + 1;
            }
            ++ln;
        }
        *ln = '\0'; 
        dest[i] = rmblk( dest[i], ln, sp );
        ++ln;
    }
    return max;
}
