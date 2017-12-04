char* RSA_encode_data(char* data,char* key,int*size_out);
char* RSA_decode_data(char* data,char* key,int*size_out);
int RSA_gen_public_key(int p,int q);
int RSA_gen_private_key(int p,int q);
