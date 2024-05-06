// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/libc/stdbool.h
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// This header file provides definitions for boolean type and constants
// in C. These definitions allow for more readable and expressive code
// by introducing standard boolean semantics into the C language, which
// lacks a built-in boolean type.
// -----------------------------------------------------------------------------

#pragma once  // Ensures the file is included only once in a single compilation.

// Defines 'bool' as an alias for 'unsigned char' to represent boolean values.
// Using 'unsigned char' ensures that the boolean occupies the smallest possible 
// amount of memory (one byte), while still being large enough to represent two states.
#define bool unsigned char

// Defines the 'true' constant as 1, conventionally used in programming to represent a true condition.
#define true 1

// Defines the 'false' constant as 0, conventionally used in programming to represent a false condition.
#define false 0
