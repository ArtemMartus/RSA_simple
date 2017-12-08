#include <iostream>
#include <string.h>
#include "utils.h"
#include "main.h"
#include "excep.h"
#include "rsa.h"

static const char* KEYFILE_IO_ERROR = "Keyfile error";

void GenerateKeys(program_arguments*args)
{
    int ret = 0;
    FILE*f=0;
    /** check key file **/
    if(!args->key_file_present){
	bzero(args->key_file,sizeof(args->key_file));
	snprintf(args->key_file,sizeof(args->key_file),"%s","keyfile.rkey");
    }

    f = fopen(args->key_file,"w");
    if(f==0)
	throw new IOException(KEYFILE_IO_ERROR,"cannot open keyfile for writing.");

    /** get main two numbers to generate keys **/
    unsigned int a = 0;
    unsigned int b = 0;
    printf("Enter A(hex) = ");
    scanf("%x",&a);

    printf("Enter B(hex) = ");
    scanf("%x",&b);

    /** generate and write keys to file **/
    key_file_struct kFile = key_file_struct();

    long pub_key = RSA_gen_public_key(a,b).key;
    long pri_key = RSA_gen_private_key(a,b).key;

    /** write file by some signature **/

    kFile.key1 = pub_key;
    kFile.key2 = pri_key;

    printf("generate_keys trying to retrieve string formated data...\n");
    char* data = kFile.get();
    printf("generate_keys messed up it into one string for file:\n%s\n",data);
    ret = fwrite((void*)data,strlen(data),1,f);
    delete[]data;

    fclose(f);
    if(ret==0)
	throw new IOException(KEYFILE_IO_ERROR,"cannot write data block.");
    printf("Successfully written keys to %s!\n",args->key_file);
}


