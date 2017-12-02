#include <iostream>
#include "rsa.h"
#include "excep.h"
using namespace std;

bool generate_keys = false;
bool encode_data = false;
bool decode_data = false;
bool key_file_present = false;
bool data_file_present = false;
char key_file[256]={0};
char data_file[256]={0};

int main(int argc,char**argv)
{
    if(argc==1)
	throw new ArgumentException(ARG_ABSENT,"Use --help to get help information.\n");
    
    for(int i = 1;i<argc;++i)
    {
	char* arg  = argv[i];
	cout<<"Argument "<<i<<" =\t\t"<<arg<<endl;
	if(_stricmp(arg,"-key")==0)
	{
	    ++i;
	    if(i >= argc)
		throw new ArgumentException(ARG_BADINPUT,"no key specified after -key command.");
	    strncpy(key_file,argv[i],sizeof(key_file)-1);
	    /** check whether we can read file **/
	    FILE* fo = fopen(key_file,"r");
	    if(fo==NULL)
		fo = fopen(key_file,"w"), if(fo==NULL)
		    throw new FileException(ARG_BADFILE,"cannot open/create key file.");
	    fclose(fo);
	    key_file_present = true;
	    continue;
	}
	if(_stricmp(arg,"-gen")==0)
	{
	    generate_keys = true;
	    continue;
	}
	if(_stricmp(arg,"-encode")==0)
	{
	    encode_data = true;
	    continue;
	}
	if(_stricmp(arg,"-decode")==0)
	{
	    decode_data = true;
	    continue;
	}
	if(_stricmp(arg,"-data")==0)
	{
	    ++i;
	    if(i>=argc)
		throw new ArgumentException(ARG_BADINPUT,"no data file specified after -data command.");
	    strncpy(data_file,argv[i],sizeof(data_file)-1);
	    FILE* fo = fopen(data_file,"r");
	    if(fo==NULL)
		fo = fopen(data_file,"w"),if(fo==NULL)
		    throw new FileException(ARG_BADFILE,"cannot open/create data file.");
	    fclose(fo);
	    data_file_present = true;
	    continue;
	}
    }
    /** TODO:
      Write handler, main functions, algorithm in its lowest resolution then go fill in rsa.(h/cpp) and excep.(h/cpp).
      Move all logic in specific functions so main(int,char**) only contain exceptions catching.
      rsa.(h/cpp) must contain detailed realization of RSA algorithm with comments.
      exep.(h/cpp) must contain detailed realization of Exceptions.
      **/
}
