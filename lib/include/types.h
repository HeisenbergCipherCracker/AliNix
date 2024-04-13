#ifndef __CACTUSOSLIB__TYPES_H
#define __CACTUSOSLIB__TYPES_H


#ifdef __cplusplus
namespace LIBHeisenKernel
{
    #define STATIC_ASSERT(condition) typedef char p__LINE__[ (condition) ? 1 : -1];

    typedef char                        int8_t;
    typedef unsigned char               uint8_t;
    typedef short                       int16_t;
    typedef unsigned short              uint16_t;
    typedef int                         int32_t;
    typedef unsigned int                uint32_t;
    typedef long long int               int64_t;
    typedef unsigned long long int      uint64_t;
    typedef unsigned long long          uintptr_t;
    typedef uint32_t size_t; 
    typedef unsigned char byte_t;
    typedef unsigned int dword_t;
    typedef long unsigned int word_t;
    typedef signed short sint16_t;
    typedef signed char sint8_t;
    typedef unsigned int sint32_t;
    typedef signed long long int sint64_t;
    typedef	long	time_t;


    constexpr uint32_t operator"" _KB(unsigned long long no)
    {
        return no * 1024;
    }
    constexpr uint32_t operator"" _MB(unsigned long long no)
    {
        return no * (1024_KB);
    }
    constexpr uint32_t operator"" _GB(unsigned long long no)
    {
        return no * (1024_MB);
    }
}

#else 
// typedef char                        int8_t;
typedef unsigned char               uint8_t;
typedef short                       int16_t;
typedef unsigned short              uint16_t;
typedef int                         int32_t;
typedef unsigned int                uint32_t;
typedef long long int               int64_t;
typedef unsigned long long int      uint64_t;
// typedef unsigned long long          uintptr_t;
// typedef uint32_t size_t; 
typedef unsigned char byte_t;
typedef unsigned int dword_t;
typedef long unsigned int word_t;
typedef	long	time_t;

#endif

// extern "C"{

//     typedef char                        int8_t;
//     typedef unsigned char               uint8_t;
//     typedef short                       int16_t;
//     typedef unsigned short              uint16_t;
//     typedef int                         int32_t;
//     typedef unsigned int                uint32_t;
//     typedef long long int               int64_t;
//     typedef unsigned long long int      uint64_t;
//     typedef unsigned long long          uintptr_t;
//     typedef uint32_t size_t; 
//     typedef unsigned char byte_t;
//     typedef unsigned int dword_t;
//     typedef long unsigned int word_t;
// }

#endif