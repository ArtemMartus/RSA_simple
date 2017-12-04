#include <iostream>
#include "constants.h"
#include "utils.h"
#include "rsa.h"
#include "excep.h"

char* RSA_encode_data(char* data,char* key,int*size_out)
{
    char* encoded_data = new char[256];
    snprintf(encoded_data,256,"%s","S o M e  En Code D  T e x t");
    return encoded_data; 
}

char* RSA_decode_data(char* data,char* key,int*size_out)
{
    char* encoded_data = new char[256];
    snprintf(encoded_data,256,"%s","S o M e  En Code D  T e x t");
    return encoded_data;
}

int RSA_gen_public_key(int p,int q)
{
    return 6;
}

int RSA_gen_private_key(int p,int q)
{
    return 7;
}
