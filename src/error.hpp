#include <stdio.h>
#include "pico/stdio.h"
#include "pico/cyw43_arch.h"

/**
 * \file error.hpp
 * Defines some debug helpers that will print to USB,
 * useful crashing or core-ing.
*/

/**
 * \fn void __printf_and_abort(const char* f, const int i)
 * 
 * \brief Dump some debug info and crash.
 * \param f a C string for error/crash location.
 * \param i a line number to be more specific on location
 * \note Call this, and program flow stops, calling abort after logging. 
 *  Keep for desperate unrecoverable states.
*/
void printf_and_abort__(const char* f, const int i){
    printf("%s:%d: Aborting now.", f, i);
    abort();
}

#ifdef NDEBUG

/**
 * \def on_error()
 * Effective only in Debug build.
 * Wraps around __PRETTY_FUNCTION and __LINE__ and calls __printf_and_abort.
 * See lower for Release definition
 * 
*/
#   define on_error() printf_and_abort__((__PRETTY_FUNCTION__), (__LINE__))
#else
/**
 * \def on_error()
 * In Debug build: __printf_and_abort(__PRETTY_FUNCTION__,__LINE__).
 * In Release build: abort silently.
*/
#   define on_error() abort()
#endif