// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/libc/limits.h
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// This header file defines limits of integral types that are used
// throughout the operating system. Defining these limits helps to 
// prevent overflows and underflows by providing a consistent reference
// for the maximum capacities of standard data types.
// -----------------------------------------------------------------------------

#pragma once  // Ensures the file is included only once in a single compilation.

// Maximum value for an int.
// This defines the upper limit for the range of values that can be stored in an 
// integer variable, which corresponds to the maximum possible value for a 32-bit 
// signed integer, ensuring that operations involving integers do not exceed this limit.
#define INT_MAX 2147483647
