
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "csvln.h"

int main( int argc, char** argv ) {

    if ( 3 > argc ) {
        printf( "%s%s%s\n", "Usage: ", *argv, " inputfile.csv outputfile.xml [separator char]" );
        return 0;
    }

    int const sp = argc > 3 ? argv[2][0] : ',';

    FILE* infd = fopen( argv[1], "r" );
    if ( NULL == infd ) {
        fputs( "The file cannot be open.\n", stderr );
        return -1;
    }

    int err = -1;

    FILE* outfd = fopen( argv[2], "w" );
    if ( NULL == outfd ) {
        fputs( "The file cannot be open.\n", stderr );
        goto closeIn;
    }

    char* headerln = NULL;
    size_t size = 0;

    if ( -1 == getline( &headerln, &size, infd ) ) {
        fputs( "The header cannot be read.\n", stderr );
        goto end;
    }

    enum { max = 32 };
    char* header[ max ];
    int const headerlen = csvln( header, max, headerln, sp );    
    if ( 0 == headerlen ) {
        fputs( "The columns of header cannot be read.\n", stderr );
        goto end;
    }

    char* line = NULL;
    size = 0;

    fputs( "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<xml>\n", outfd );

    int id = 0;
    while( -1 != getline( &line, &size, infd ) ) {

        char* entry[ max ];
        int num = csvln( entry, max, line, sp );        
        if ( 0 >= num || ( 1 == num && 0 == strlen( entry[0] ) ) )
            continue;

        if ( num > headerlen )
            num = headerlen;

        fprintf( outfd, "%s%d%s", "\t<entry id=\"", id++, "\">\n" );
        for( int i = 0; i < num; ++i )
            if ( strlen( entry[0] ) )
                fprintf( outfd, "%s%s%s%s%s", "\t\t<field name=\"", header[i], "\">",  entry[i], "</field>\n" );
        fputs( "\t</entry>\n", outfd );
    }  
    
    fputs( "</xml>\n", outfd );

    err = 0;

  end:

    if ( NULL != headerln )
        free( headerln );

    if ( NULL != line )    
        free( line );

  closeIn:
    fclose( infd );

    return err;
}