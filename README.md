# HashCash-like exercise

Create a program that returns the bytes that, after applying the “or” operator with an input string, result in an MD5 hash that start with a certain number (example = 8) of zeroes.
 
MD5 of cash is 93585797569d208d914078d513c8c55a

The combined md5 is 007da27771c47ce1253057d90e87fdd6

The combined md5 in bit representation is: 000000000101111100010001010111011100100011100001000110001111100100001110101001000000011000111010100100110110011100000111000010101111110011010110

Hash conflict was found with: 0, 2, 8, 135,
 
So the desired answer in this specific case is (e.g.) 0,2,8,135 (but this is not the only correct answer), because the md5 of “cash” | {0,2,8,35} is “007da…”, e.g. the first 2 hex-characters (together 256 bits = 8 zeros) are zero.

Simply build with build.sh and run the result executable with your string argument.
