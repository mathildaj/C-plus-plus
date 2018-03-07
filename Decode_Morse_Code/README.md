# Project Description

This is a C++ application. It decrypts an encrypted file using C++ Bitsets.

1. The file to be read is encrypted using Morse Code as a basis for the encryption. Each Letter/Number/Punctuation character in Morse Code consists of Dots or Dashes.
For example,the 'C' in Morse Code is: "..".

Based on this, each character can be represented by 2 bit patterns: Dash [ 01 ]
and Dot [ 10 ]. So the letter 'C' would be converted to Morse Code as: 10011001 [ 168 ]. The reason two bits are
necessary, is it's also necessary to represent a Character Termination Symbol [ 00 ], and a word terminator [ 11 ].

The file includes the following classes:

1. FileIO
2. Converter
3. Table