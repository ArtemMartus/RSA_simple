#include <iostream>
#include "constants.h"
#include "utils.h"
#include "main.h"
#include "rsa.h"
#include "excep.h"
using namespace std;


/** TODO:
  1) Finish encoding\decoding functions
  2) Write encoding\decoding\generate\parse function realization into separate files
  3) Write helping data & function names into main.h
  4) Write rsa.(h\cpp)
  5) Write excep.(h\cpp)
 **/

/** main function **/
int main(int argc,char**argv)
{
    program_arguments* args = 0;
    /** surround it with try{}catch(...){} to capture all exceptions **/
    try{
	InitArguments(argc,argv,&args);

	if(args->generate_keys)
	    GenerateKeys(args);
	else if(args->encode_data)
	    EncodeData(args);
	else if(args->decode_data)
	    DecodeData(args);

    }
    catch( Exception* e)
    {
	printf("Exception handler called!\nException details:%s\nQuitting...\n",e->what());
	delete e;
    }

    delete args;

    printf("Thank you for using this awesome application!\nHave fun and keep your information safe with us!\n\n");
    return 0;
}

