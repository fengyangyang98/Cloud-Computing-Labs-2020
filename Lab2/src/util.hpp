#ifndef __UTIL_HPP
#define __UTIL_HPP

#include "global.h"
#include "iostream"
#include <getopt.h>

void GetOptLong(int argc, char *argv[]);
namespace this_debug
{
    void show_config();
} // namespace debug

void mem_free();

#endif // __UTIL_HPP