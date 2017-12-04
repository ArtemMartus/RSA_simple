#include <iostream>
#include <string.h>
#include "constants.h"
#include "utils.h"
#include "main.h"
#include "excep.h"
#include "rsa.h"

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
    int a = 0;
    int b = 0;
    printf("Enter A = ");
    scanf("%d",&a);

    printf("Enter B = ");
    scanf("%d",&b);

    /** generate and write keys to file **/
    key_file_struct kFile = key_file_struct();

    int pub_key = RSA_gen_public_key(a,b);
    int pri_key = RSA_gen_private_key(a,b);

    /** write file by some signature **/

    kFile.key1 = pub_key;
    kFile.key2 = pri_key;

    char* data = kFile.get();
    ret = fwrite((void*)data,strlen(data)+sizeof('\0'),1,f);
    delete[]data;

    fclose(f);
    if(ret==0)
	throw new IOException(KEYFILE_IO_ERROR,"cannot write data block.");
    printf("Successfully written keys to %s!\n",args->key_file);
}


