union RSA_Key
{
    struct{
	unsigned int a;
	unsigned int b;
    };
    unsigned long key;
};
char* RSA_encode_data(char* data,char* key,int*size_out);
char* RSA_decode_data(char* data,char* key,int*size_out);
RSA_Key RSA_gen_public_key(unsigned int p,unsigned int q);
RSA_Key RSA_gen_private_key(unsigned int p,unsigned int q);
