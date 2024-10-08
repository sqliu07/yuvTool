#ifndef __os_h__
#define __os_h__


#define S_OK          0
#define S_ERROR      -1
#define TRUE          1
#define FALSE         0

typedef void        VOID;
typedef bool        BOOL;
typedef int8_t      INT8;
typedef int16_t     INT16;
typedef int32_t     INT32;
typedef int64_t     INT64;
typedef uint8_t     UINT8;
typedef uint16_t    UINT16;
typedef uint32_t    UINT32;
typedef uint64_t    UINT64;
typedef float       FLOAT;
typedef double      DOUBLE;



#include "yuvTool.h"
#ifdef __LINUX__
#include <cstdio>
#include <iostream>
#ifdef __HAS_LOG__
#define LOG_VERBOSE(...) do { \
    printf(__VA_ARGS__); \
    std::cout << std::endl; \
} while (0)
#define LOG_ERROR(...) do { \
    printf(__VA_ARGS__); \
    std::cout << std::endl; \
} while (0)
#else
#define LOG_VERBOSE(fmt,...)
#define LOG_ERROR(fmt, ...)
#endif

#endif

#ifdef __ANDROID__
#include <jni.h>
#include <android/log.h>

#ifdef __HAS_LOG__
#ifndef LOG_ERROR
#define LOG_ERROR(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#ifndef LOG_VERBOSE
#define LOG_VERBOSE(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#endif
#else

#ifndef LOG_ERROR
#define LOG_ERROR(fmt, ...)   
#endif

#ifndef LOG_VERBOSE
#define LOG_VERBOSE(fmt, ...)
#endif

#endif//__HAS_LOG__

#else

#ifndef LOG_ERROR
#define LOG_ERROR(fmt, ...)
#endif

#ifndef LOG_VERBOSE
#define LOG_VERBOSE(fmt, ...)
#endif

#endif//__ANDROID__

#ifdef __cplusplus

#ifdef __linux
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

#ifdef _STDINT_H
#undef _STDINT_H
#endif // _STDINT_H
#include <stdint.h>

#endif // __linux

extern "C" {
#endif //__cplusplus

#ifdef __cplusplus
}
#endif// __cplusplus

#endif
