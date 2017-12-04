#include <iostream>
#include "constants.h"
#include "utils.h"
#include <string.h>

program_arguments::program_arguments():generate_keys(0),encode_data(0),decode_data(0),
    key_file_present(0),data_file_present(0)
{
    bzero(key_file,max_path_size);
    bzero(data_file,max_path_size);
}

key_file_struct::key_file_struct(){
    key1 = key2 = 0;
}

void key_file_struct::set(char*data)
{
    sscanf(data,"%d\n%d\n",&key1,&key2);
}

char* key_file_struct::get()
{
    size_t needed_memory = sprintf(0,"%d\n%d\n",key1,key2)+sizeof('\0');
    char* data = new char[needed_memory];
    snprintf(data,needed_memory,"%d\n%d\n",key1,key2);
    return data; /** do not forget to delete[]data !!! **/
}
