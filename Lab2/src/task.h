#pragma once

#include "global.h"
#include "cthreadpool.h"
#include "transport.hpp"

class SocketTask : public CTask
{
private:
    /* data */
public:
    SocketTask(/* args */) = default;
    void run() {
        
    }
    ~SocketTask() {}
};

SocketTask::SocketTask(/* args */)
{
}

SocketTask::~SocketTask()
{
}
