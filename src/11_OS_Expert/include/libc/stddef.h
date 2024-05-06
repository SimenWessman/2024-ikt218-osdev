// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/libc/stddef.h
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
//
// Description: 
// This header file provides fundamental type definitions and macros
// that are essential for system development. It includes the definition
// of NULL, a macro representing the null pointer constant. This definition
// is crucial for pointer validity checks and controlling pointer behavior
// throughout the operating system and applications.
// -----------------------------------------------------------------------------

#pragma once  // Ensures the file is included only once in a single compilation.

// Defines NULL as a cast of 0 to a void pointer, standardizing the representation
// of a null pointer in the system. This is used in pointer operations to indicate 
// that the pointer does not point to any object or function.
#define NULL ((void*)0)
