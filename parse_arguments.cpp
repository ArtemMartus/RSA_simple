#include <iostream>
#include <string.h>
#include "constants.h"
#include "utils.h"
#include "main.h"
#include "excep.h"
#include "rsa.h"

void ParseArguments(int argc,char**argv,program_arguments * args)
{
    if(argc==1)
	throw new ArgumentException(ARG_ABSENT,"Use --help to get help information.\n");

    /** Flick through all programm arguments passed except first one **/

    for(int i = 1;i<argc;++i)
    {
	char* arg  = argv[i];
	std::cout<<"##Argument "<<i<<" =\t\t"<<arg<<std::endl;
	/** Handle '-key' argument **/
	if(strcmp((const char*)arg,"-key")==0)
	{
	    ++i; /** following argument MUST be path to key file **/
	    if(i >= argc)
		throw new ArgumentException(ARG_BADINPUT,"no key specified after -key command.");

	    snprintf(args->key_file,sizeof(args->key_file),"%s",argv[i]);

	    /** check whether we can read file **/
	    FILE* fo = fopen(args->key_file,"r");
	    if(fo==NULL)
		fo = fopen(args->key_file,"w");
	    if(fo==NULL)
		throw new IOException(ARG_BADFILE,"cannot open/create key file.");
	    fclose(fo);
	    args->key_file_present = true;
	    continue;
	}
	/** Handle -gen(erate keys) argument **/
	if(strcmp((const char*)arg,"-gen")==0)
	{
	    args->generate_keys = true;
	    continue;
	}
	/** Handle -encode file argument **/
	if(strcmp((const char*)arg,"-encode")==0)
	{
	    args->encode_data = true;
	    continue;
	}
	/** Handle -decode file argument **/
	if(strcmp((const char*)arg,"-decode")==0)
	{
	    args->decode_data = true;
	    continue;
	}
	/** Handle -data argument **/
	if(strcmp((const char*)arg,"-data")==0)
	{
	    /** following argument MUST be path to file on which data we need to make some work **/ 
	    ++i;
	    if(i>=argc)
		throw new ArgumentException(ARG_BADINPUT,"no data file specified after -data command.");
	    snprintf(args->data_file,sizeof(args->data_file),"%s",argv[i]);
	    FILE* fo = fopen(args->data_file,"r");
	    if(fo==NULL)
		fo = fopen(args->data_file,"w");
	    if(fo==NULL)
		throw new IOException(ARG_BADFILE,"cannot open/create data file.");
	    fclose(fo);
	    args->data_file_present = true;
	    continue;
	}
    }
}


