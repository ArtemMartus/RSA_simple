#include <iostream>
#include "excep.h"

Exception::Exception(const char* code_,const char* description_):code(code_),description(description_)
{
    error_data = 0;
}

Exception::~Exception()
{
    if(error_data)
	delete[]error_data;
}

char* Exception::what()
{
    return error_data;
}

IOException::IOException(const char* code_, const char* description_):Exception(code_,description_)
{
    int size = snprintf(0,0,"IOException:%s!\n%s\n",code,description);
    error_data = new char[size];
    snprintf(error_data,size,"IOException:%s!\n%s\n",code,description);
}

ArgumentException::ArgumentException(const char* code_,const char* description_):Exception(code_,description_)
{
    int size = snprintf(0,0,"ArgumentException:%s!\n%s\n",code,description);
    error_data = new char[size];
    snprintf(error_data,size,"ArgumentException:%s!\n%s\n",code,description);
}

