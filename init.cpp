#include <iostream>
#include "constants.h"
#include "utils.h"
#include "main.h"

void InitArguments(int argc,char**argv,program_arguments** /**out**/ args) /** initialize program_arguments structure **/
{
    program_arguments *arg = new program_arguments();
    ParseArguments(argc,argv,arg);
    *args=arg;
}
