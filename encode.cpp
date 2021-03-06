#include <iostream>
#include <string.h>
#include "utils.h"
#include "main.h"
#include "excep.h"
#include "rsa.h"

static const char* KEYFILE_IO_ERROR = "Keyfile error";
static const char* DATA_IO_ERROR = "Datafile error";

void EncodeData(program_arguments* args)
{
    /** Firstly check whether we have data file to proceed
      if yes:
      encode it and write encoded data to stdout + try to write it in %data_file%_encoded
      if no:
      wait for data from stdin and encode it
     **/

    char*data = 0;
    int data_size = 0;

    char* key = 0;
    int key_size = 0;

    if(args->key_file_present)
    {
	int ret = 0;
	int file_size = 0;
	FILE *kF = fopen(args->key_file,"r");
	if(kF==0)
	    throw new IOException(KEYFILE_IO_ERROR,"cannot open keyfile for reading.");
	fseek(kF,0,SEEK_END);
	file_size = ftell(kF);
	key_size = file_size;
	rewind(kF);
	printf("Working on %s. %d bytes to process...\n",args->key_file,file_size);

	/** Load data file into memory **/
	key = new char[file_size];
	ret = fread((void*)key,1,file_size,kF);	

	if(ret == 0)
	    throw new IOException(KEYFILE_IO_ERROR,"cannot read public key from file.");
	fclose(kF);
	printf("Successfully loaded public key into memory!\n");
    }
    else
	throw new ArgumentException(KEYFILE_IO_ERROR,"you have not specified keyfile with right keys.");

    if(args->data_file_present)
    {
	FILE * f = 0;
	int ret = 0;
	int file_size = 0;

	f = fopen(args->data_file,"r");
	if(f == 0)
	    throw new IOException(DATA_IO_ERROR,"cannot open read stream from data file.");
	/** Obtain file size **/
	fseek(f,0,SEEK_END);
	file_size = ftell(f);
	data_size = file_size;
	rewind(f);
	printf("Working on %s. %d bytes to process...\n",args->data_file,file_size);

	/** Load data file into memory **/
	data = new char[file_size];
	ret = fread((void*)data,1,file_size,f);	

	if(ret == 0)
	    throw new IOException(DATA_IO_ERROR,"cannot read data from file to encode.");
	fclose(f);
	printf("Successfully loaded data into memory!\n");
    }
    else
       throw new ArgumentException(DATA_IO_ERROR,"you have not specified file to encode data from.");

    printf("Encoding...\n");

    int encoded_data_size = 0;
    char* encoded_data = RSA_encode_data(data,key,&encoded_data_size);

    printf("Encoded data:\n========================================\n\n%s\n\n========================================\n",encoded_data);

    /** Now try to write it to the file **/

    FILE * f_out = 0;
    int ret = 0;
    char enc_data_file[256]={0};

    snprintf(enc_data_file,sizeof(enc_data_file),"%s_encoded",args->data_file);
    f_out = fopen(enc_data_file,"w");
    if(f_out == 0)
	throw new IOException(DATA_IO_ERROR,"cannot open write stream for encrypted data file.");
    ret = fwrite((void*)encoded_data,1,encoded_data_size,f_out);
    if(ret == 0)
	throw new IOException(DATA_IO_ERROR,"cannot write encrypted data block to a file.");
    fclose(f_out);

    delete [] data;
    delete [] encoded_data;
    delete [] key;

    printf("Successfully encoded data and wrote it to %s!\n",enc_data_file);
}


