#ifndef httpParse_HPP
#define httpParse_HPP

#include "global.h"
#include <fstream>

using std::map;

void apart(const char* result, map<string, string> post,string s);   //��� 
void connect(char* filename,char* request,char* header,string s);   //���� 

#endif 
