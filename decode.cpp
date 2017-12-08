#include <iostream>
#include <string.h>
#include "utils.h"
#include "main.h"
#include "rsa.h"
#include "excep.h"

static const char* KEYFILE_IO_ERROR = "Keyfile error";
static const char* DATA_IO_ERROR = "Datafile error";

void DecodeData(program_arguments*args)
{
    /** Firstly check whether we have data file to proceed
      if yes:
      decode it and write decoded data to stdout + try to write it in %data_file%_decoded
      if no:
      wait for data from stdin and decode it
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

	/** Load key file into memory **/
	key = new char[file_size];
	ret = fread((void*)key,1,file_size,kF);	

	if(ret == 0)
	    throw new IOException(KEYFILE_IO_ERROR,"cannot read private key from file.");
	fclose(kF);
	printf("Successfully loaded private key into memory!\n");
    }
    else
    {	key = new char[256];
	bzero(data,256);

	printf("You have not specified key file!\nEnter private key to encode(2B maximum):");
	fgets(key,256,stdin);
	key[256-1]='\0';
    }

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
	printf("Woring on %s. %d bytes to process...\n",args->data_file,file_size);

	/** Load data file into memory **/
	data = new char[file_size];
	ret = fread((void*)data,1,file_size,f);	

	if(ret == 0)
	    throw new IOException(DATA_IO_ERROR,"cannot read data from file to decode.");
	fclose(f);
	printf("Successfully loaded data into memory!\n");
    }
    else
    {
	data = new char[256];
	bzero(data,256);

	printf("You have not specified data file!\nEnter data to decode(8kB maximum):");
	fgets(data,256,stdin);
	data[256-1]='\0';
    }
    printf("Decoding...\n");

    int decoded_data_size = 0;
    char* decoded_data = RSA_decode_data(data,key,&decoded_data_size);

    printf("Decoded data:\n========================================\n\n%s\n\n========================================\n",decoded_data);

    /** Now try to write it to the file **/

    FILE * f_out = 0;
    int ret = 0;
    char dec_data_file[256]={0};

    snprintf(dec_data_file,sizeof(dec_data_file),"%s_decoded",args->data_file);
    f_out = fopen(dec_data_file,"w");
    if(f_out == 0)
	throw new IOException(DATA_IO_ERROR,"cannot open write stream for encrypted data file.");
    ret = fwrite((void*)decoded_data,1,decoded_data_size,f_out);
    if(ret == 0)
	throw new IOException(DATA_IO_ERROR,"cannot write encrypted data block to a file.");
    fclose(f_out);

    delete [] data;
    delete [] key;
    delete [] decoded_data;

    printf("Successfully decoded data and wrote it to %s!\n",dec_data_file);
}
