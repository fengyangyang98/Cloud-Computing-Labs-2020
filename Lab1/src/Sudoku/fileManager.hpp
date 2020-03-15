#ifndef FILEMANAGER_HPP
#define FILEMANAFER_HPP

#include <string>
#include <string.h>

std::string problemReader();

int problemSemInit();
void * problemReadThread(void * arg);
void * filenameReadThread(void * arg);

#endif