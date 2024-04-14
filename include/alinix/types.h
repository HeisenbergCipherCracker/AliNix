/**
 * MIT License
 *
 * Copyright (c) 2024 Heisenberg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * @author Heisenberg
 * @file types.h

 */
#ifndef __ALINIX_KERNEL__TYPES_H_HEADER
#define __ALINIX_KERNEL__TYPES_H_HEADER_

#include "_types.h"


typedef char                        int8_t;
typedef unsigned char               uint8_t;
typedef short                       int16_t;
typedef unsigned short              uint16_t;
typedef int                         int32_t;
typedef unsigned int                uint32_t;
typedef long long int               int64_t;
typedef unsigned long long int      uint64_t;
typedef unsigned long long          uintptr_t;
typedef signed char               sint8_t;
typedef signed short              sint16_t;
typedef signed int               sint32_t;
typedef signed long long int      sint64_t;
//! This might run into problem (size_t)
typedef unsigned long int size_t;
#if !defined(__cplusplus)
//TODO Fix here
typedef enum  {false = 0, true = 1} bool;
#define true 1
#define false 0
#endif

#ifndef NULL
#define NULL ALINIX_NULL
#endif

#if !defined(ALINIX_NULL_CPP_NULL_PTR) && defined(__cplusplus)
#define  ALINIX_NULL_CPP_NULL ALINIX_NULL_CPP_NULL_PTR

#endif /*ALINIX_NULL_CPP_NULL_PTR*/

/*Defining the operators for the File system measures*/
uint32_t KB(unsigned long long no) {
    return no * 1024;
}

uint32_t MB(unsigned long long no) {
    return no * KB(1024);
}

uint32_t GB(unsigned long long no) {
    return no * MB(1024);
}


////////////////////////////////////////
typedef struct 
{
    uint32_t signature;     // USBC in hexadecimal, acting as magic number
    uint32_t tag;           // Signature
    uint32_t transferLen;   // Number of bytes to transfer excluding size of CBW
    uint8_t flags;          // 7: 0=Out 1=In, 6:0=Reserved
    uint8_t lun;            // 7:4 Reserved, 3:0 Logical Unit Number
    uint8_t cmdLen;         // Length of command in next field [1-16]
    uint8_t command[16];    // Command Data
} __attribute__((packed)) CommandBlockWrapper;


#endif /*__ALINIX_KERNEL__TYPES_H_HEADER_*/