
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

#include <stdio.h>
#include <string.h>
#include "csvln.h"


// ----------------------------------------------------- Test "framework": ---

#define done() return 0
#define fail() return __LINE__
static int checkqty = 0;
#define check( x ) do { ++checkqty; if (!(x)) fail(); } while ( 0 )

struct test {
    int(*func)(void);
    char const* name;
};

static int test_suit( struct test const* tests, int numtests ) {
    printf( "%s", "\n\nTests:\n" );
    int failed = 0;
    for( int i = 0; i < numtests; ++i ) {
        printf( " %02d%s%-25s ", i, ": ", tests[i].name );
        int linerr = tests[i].func();
        if ( 0 == linerr )
            printf( "%s", "PASSED\n" );
        else {
            printf( "%s%d\n", "FAILED, line: ", linerr );
            ++failed;
        }
    }
    printf( "\n%s%d\n", "Total checks: ", checkqty );
    printf( "%s[ %d / %d ]\r\n\n\n", "Tests PASS: ", numtests - failed, numtests );
    return failed;
}


// ----------------------------------------------------------- Unit tests: ---


static int simplexample( void ) {
    char ln[] = "home;perry;sailor\r\n";
    char* val[ 3 ];
    int num = csvln( val, 3, ln, ';' ); 
    check( 3 == num );
    check( 0 == strcmp( val[0], "home" ) );
    check( 0 == strcmp( val[1], "perry" ) );
    check( 0 == strcmp( val[2], "sailor" ) );
    done();    
}

static int whitespaces( void ) {
    {
        char ln[] = "\thome blanca; perry \t\t  ; \t \t sailor \r\n  ";
        char* val[ 3 ];
        int num = csvln( val, 3, ln, ';' ); 
        check( 3 == num );
        check( 0 == strcmp( val[0], "home blanca" ) );
        check( 0 == strcmp( val[1], "perry" ) );
        check( 0 == strcmp( val[2], "sailor" ) );
    }
    {
        char ln[] = "    home blanca\t perry   \t sailor \r\n  ";
        char* val[ 3 ];
        int num = csvln( val, 3, ln, '\t' ); 
        check( 3 == num );
        check( 0 == strcmp( val[0], "home blanca" ) );
        check( 0 == strcmp( val[1], "perry" ) );
        check( 0 == strcmp( val[2], "sailor" ) );
    }
    done();    
}

static int quotes( void ) {
    char ln[] = "home;\t \"perry\\n\"  ; sailor \r\n";
    char* val[ 3 ];
    int num = csvln( val, 3, ln, ';' ); 
    check( 3 == num );
    check( 0 == strcmp( val[0], "home" ) );
    check( 0 == strcmp( val[1], "perry\n" ) );
    check( 0 == strcmp( val[2], "sailor" ) );
    done();    
}

static int endline( void ) {
    {
        char ln[] = "home; \"perry\\n\"  ; sailor ";
        char* val[ 3 ];
        int num = csvln( val, 3, ln, ';' ); 
        check( 3 == num );
        check( 0 == strcmp( val[0], "home" ) );
        check( 0 == strcmp( val[1], "perry\n" ) );
        check( 0 == strcmp( val[2], "sailor" ) );
    }
    {
        char ln[] = "home; \"perry\\n\"  ; sailor \n";
        char* val[ 3 ];
        int num = csvln( val, 3, ln, ';' ); 
        check( 3 == num );
        check( 0 == strcmp( val[0], "home" ) );
        check( 0 == strcmp( val[1], "perry\n" ) );
        check( 0 == strcmp( val[2], "sailor" ) );
    }
    {
        char ln[] = "home; \"perry\\n\"  ; sailor \r";
        char* val[ 3 ];
        int num = csvln( val, 3, ln, ';' ); 
        check( 3 == num );
        check( 0 == strcmp( val[0], "home" ) );
        check( 0 == strcmp( val[1], "perry\n" ) );
        check( 0 == strcmp( val[2], "sailor" ) );
    }
    done();    
} 

static int maxitems( void ) {
    char ln[] = "home;\t \"perry\\n\"  ; sailor \r\n";
    char* val[ 2 ];
    int num = csvln( val, 2, ln, ';' );
    check( 2 == num );
    check( 0 == strcmp( val[0], "home" ) );
    check( 0 == strcmp( val[1], "perry\n" ) );
    done();    
}

// --------------------------------------------------------- Execute tests: ---

int main( void ) {
    static struct test const tests[] = {
        { simplexample, "simple example" },
        { whitespaces,  "white spaces"   },
        { quotes,       "quotes"         },
        { endline,      "end line"       },
        { maxitems,    "max items"       }
    };
    return test_suit( tests, sizeof tests / sizeof *tests );
}
