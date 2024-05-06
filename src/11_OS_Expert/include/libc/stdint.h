// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/libc/stdint.h
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// This header file defines a set of typedefs that specify exact-width
// integer types, along with the `size_t` type which is commonly used for
// memory sizing and indexing. The definitions ensure consistent behavior
// of integer sizes across different platforms, which is particularly
// critical in low-level system programming where specific integer sizes
// are required to match hardware characteristics or protocol specifications.
// -----------------------------------------------------------------------------

#pragma once  // Ensures the file is included only once in a single compilation.

// Unsigned and signed fixed-width integer types.
typedef long unsigned int size_t;   // Type used for representing sizes of objects.
typedef long unsigned int uint32_t; // Unsigned 32-bit integer.
typedef unsigned short uint16_t;    // Unsigned 16-bit integer.
typedef unsigned char uint8_t;      // Unsigned 8-bit integer.

typedef long int int32_t;           // Signed 32-bit integer.
typedef short int int16_t;          // Signed 16-bit integer.
typedef signed char int8_t;         // Signed 8-bit integer.
