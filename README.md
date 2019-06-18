# csvln
[![Build Status](https://travis-ci.org/rafagafe/csvln.svg?branch=master)](https://travis-ci.org/rafagafe/csvln)

The csvln module includes a function that analyzes a line of a CSV file. This function writes to an array of pointers the start of each of the fields in the CSV file line.

To facilitate the processing of the data the returned strings are null-terminated strings. This is achieved by setting the separator characters to null character.

