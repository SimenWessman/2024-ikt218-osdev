// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/libc/stdarg.h
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description:
// This header file provides the definitions necessary to handle variable 
// argument lists in functions. It includes macros for initializing, accessing,
// and cleaning up variable argument lists, which are used in functions
// that accept an indefinite number of parameters.
// -----------------------------------------------------------------------------

#pragma once  // Ensures the file is included only once in a single compilation.

// Type used to declare a variable that will refer to each argument in turn;
// this type is defined internally by the compiler.
typedef __builtin_va_list va_list;

// Initializes a va_list to point to the first argument after the last fixed argument.
// This macro must be called first before any access to variable arguments can be performed.
//
//   @param v: The va_list to initialize.
//   @param l: The last fixed argument before the variable arguments start.
#define va_start(v, l) __builtin_va_start(v, l)

// Cleans up a va_list when done with the arguments. This macro should be executed
// before the function returns whenever va_start has been previously used in that function.
//
//   @param v: The va_list to clean up.
#define va_end(v) __builtin_va_end(v)

// Retrieves the next argument in the parameter list of the function with the specified type.
//
//   @param v: The va_list from which to retrieve the argument.
//   @param l: The type of the argument to retrieve.
#define va_arg(v, l) __builtin_va_arg(v, l)
