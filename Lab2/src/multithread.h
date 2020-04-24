#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include "global.h"
#include "transport.hpp"
#include "httpParse.hpp"

void *listen(void* arg) ;

void handle();

#endif