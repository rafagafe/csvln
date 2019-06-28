# csvln
[![Build Status](https://travis-ci.org/rafagafe/csvln.svg?branch=master)](https://travis-ci.org/rafagafe/csvln)

The csvln module includes a function that analyzes a line of a CSV file. This function writes to an array of pointers the start of each of the fields in the CSV file line.

To facilitate the processing of the data the returned strings are null-terminated strings. This is achieved by setting the separator characters to null character.

```C
/** It parses a CSV line.
  * @param dest Destination array of pointers to fields (null-terminated strings).
  * @param max  Length of the destination array.
  * @param ln   CSV line. It will be edited changing the separators or space chars by null chars.
  * @param sp   Indicates the separator char.
  * @return     Number of fields found. */
int csvln( char* dest[], int max, char* ln, int sp );
```


# Example

In this example a CSV line is parsed and its fields are printed.

```C
#include "csvln.h"
#include <stdio.h>
#include <stdlib.h>

int main( void ) {
    char line[] = " perry ,\t 2019 , \" The house is big\", 21";
    puts( line );
    char* fields[16];
    int num = csvln( fields, 16, line, ',' );
    if ( 0 > num ) {
        puts("Parser error");
        return EXIT_FAILURE;
    }
    for( int i = 0; i < num; ++i )
        printf( "[%d]: '%s'\n", i, fields[i] );
    return EXIT_SUCCESS;
}
```

Building and running of the example:

```
$ gcc -c csvln.c
$ gcc -c main.c
$ gcc -o example main.o csvln.o
$ ./example
 perry ,         2019 , " The house is big", 21
[0]: 'perry'
[1]: '2019'
[2]: ' The house is big'
[3]: '21'
$ 
```