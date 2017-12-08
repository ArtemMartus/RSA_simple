#include <iostream>
#include "utils.h"
#include <string.h>
#include "rsa.h"

program_arguments::program_arguments():generate_keys(0),encode_data(0),decode_data(0),
    key_file_present(0),data_file_present(0)
{
    bzero(key_file,sizeof(key_file));
    bzero(data_file,sizeof(key_file));
}

key_file_struct::key_file_struct(){
    key1 = key2 = 0;
}

void key_file_struct::set(char*data)
{
    sscanf(data,"%lx, %lx",&key1,&key2);
}

char* key_file_struct::get()
{
    size_t needed_memory = snprintf(0,0,"%lx, %lx",key1,key2)+sizeof('\0');
    RSA_Key k;
    k.key = key1;
    printf("RSA_Key key1->{%x,%x}\n",k.a,k.b);
    k.key = key2;
    printf("RSA_Key key2->{%x,%x}\n",k.a,k.b);
    printf("key_file_struct::get() needed_memory=%d\n",needed_memory);
    char* data = new char[needed_memory];
    snprintf(data,needed_memory,"%lx, %lx",key1,key2);
    printf("key_file_struct::get() returned %s\n",data);
    return data; /** do not forget to delete[]data !!! **/
}
