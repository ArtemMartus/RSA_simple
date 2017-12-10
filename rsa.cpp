#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>
#include "utils.h"
#include "rsa.h"
#include "excep.h"

using namespace std;

void get_simple(unsigned int &a,unsigned int& b)
{
    unsigned int i =255&(a>b?b:a);
    printf("get_simple start: i=%d\n",i);
    while(true)
    {
	printf("get_simple main: i=%d,a=%d,b=%d\n",i,a,b);
	if(i==0)
	{
	    char err_msg[512]={0};
	    snprintf(err_msg,512,"##i=%d,a=%d,b=%d\n",i,a,b);
	    throw new ArgumentException("get_simple fault",err_msg);
	}
	if(i==1)
	    break;

	int _a = a%i;
	int _b = b%i;
	if(_a==0&&_b==0)
	{
	    a /= i;
	    b /= i;
	    continue;
	}
	--i;
    }
}

bool is_2_simple(unsigned int a,unsigned int b)
{
    int i =255&(a>b?b:a);
    while(true)
    {
	printf("is_2_simple looping: i=%d,a=%d,b=%d\n",i,a,b);
	if(i==0)
	    throw new ArgumentException("is_2_simple fault","get_simple bad input.");
	if(i==1)
	    return true;

	int _a = a%i;
	int _b = b%i;
	if(_a==0&&_b==0)
	    return false;
	--i;
    }
}

bool is_simple(unsigned int a)
{
    unsigned int i = a-1;
    if(i<=2)
	return true;
    while(true)
    {
	if(i==1)
	    return true;
	if(a%i == 0)
	    return false;
	i--;
    }
}

unsigned int choose_e(unsigned int e)
{
    for(unsigned int i = e-1;i>0 ;--i)
    {
	printf("choose_e looping: i=%d\n",i);
	if(!is_simple(i))
	    continue;
	printf("choose_e %d is simple!\n",i);
	if(!is_2_simple(i,e))
	    continue;
	printf("choose_e %d is perfect!\n",i);
	return i;
    }
    throw new ArgumentException("choose_e fault","cannot find any e (choose another entry values).");
    return 0;
}

unsigned int choose_d(unsigned int e,unsigned int euler)
{
    printf("choose_d start: e=%d,euler=%d\n",e,euler);
    for(int i = 4096;i >1;--i)
    {
	int val = i*e % euler;
	printf("choose_d loop: i=%d,val=%d\n",i,val);
	if(val==1){
	    printf("choose_d %d is perfect!\n",i);
	    return i;
	}
    }
    throw new ArgumentException("choose_d fault","idk what's going on, rly.");
    return 0;
}

unsigned int encode_int(unsigned int val,unsigned int e,unsigned int n)
{
    unsigned int power = 1;
    val %= n;
    while(e>0){
	if(e&1) power = (power*val)%n;
	val=(val*val)%n;
	e >>= 1;
    }

    printf("encode_int power=%x\n",power);
    return power;
}

unsigned int decode_int(unsigned int val,unsigned int d,unsigned int n)
{
    unsigned int power = 1;
    val %= n;
    while(d>0){
	if(d&1) power = (power*val)%n;
	val=(val*val)%n;
	d >>= 1;
    }

    printf("decode_int power=%x\n",power);
    return power;
}

char* RSA_encode_data(char* data,char* key,int*size_out)
{
    int data_len = strlen(data);

    char* encoded_data = new char[data_len+1];
    bzero(encoded_data,data_len+1);

    key_file_struct keys;
    keys.set(key);
    RSA_Key pub;
    pub.key = keys.key1;
    
    for(int i = 0;i < data_len;++i)
    {
	unsigned int tmp = encode_int((unsigned int)data[i],pub.a,pub.b);
	if(tmp>255)
	    printf("### tmp val greater than 255(%x), some errors may occur.\n",tmp);
	encoded_data[i] = (char)(unsigned char)(tmp&0xff);
	printf("RSA_encode_data: looping i=%x,last char=%u,new char=%u\n",i,(unsigned int)data[i],(unsigned int)encoded_data[i]);
    }	
    if(size_out)
	*size_out = data_len+1;
    return encoded_data; 
}

char* RSA_decode_data(char* data,char* key,int*size_out)
{
    int data_len = strlen(data);

    char* decoded_data = new char[data_len+1];
    bzero(decoded_data,data_len+1);

    key_file_struct keys;
    keys.set(key);
    RSA_Key pri;
    pri.key = keys.key2;

    for(int i = 0;i < data_len;++i)
    {
	unsigned int tmp = decode_int((unsigned int)data[i],pri.a,pri.b);
	if(tmp>255)
	    printf("### tmp val greater than 255(%x), some errors may occur.\n",tmp);
	decoded_data[i] = (char)(unsigned char)(tmp&0xff);
	printf("RSA_decode_data: looping i=%x,last char=%u,new char=%u\n",i,(unsigned int)data[i],(unsigned int)decoded_data[i]);
    }	
    if(size_out)
	*size_out = data_len+1;
    return decoded_data;   
}

RSA_Key RSA_gen_public_key(unsigned int p,unsigned int q)
{
    printf("gen_pub_key start: p=%d,q=%d\n",p,q);
    RSA_Key pub;
    get_simple(p,q);
    printf("gen_pub_key main: p=%d,q=%d\n",p,q);
    pub.b = p*q;
    unsigned int euler = (p-1)*(q-1);
    printf("gen_pub_key euler=%d\n",euler);
    pub.a = choose_e(euler);
    printf("gen_pub_key e=%d\n",pub.a);
    printf("gen_pub_key your key {%x,%x}\n",pub.a,pub.b);
    return pub;
}

RSA_Key RSA_gen_private_key(unsigned int p,unsigned int q)
{
    printf("gen_priv_key start: p=%d,q=%d\n",p,q);
    RSA_Key pri;
    RSA_Key pub = RSA_gen_public_key(p,q);
    printf("gen_priv_key main: choose private key-part\n");
    pri.a = choose_d(pub.a,(p-1)*(q-1));
    pri.b = pub.b;
    printf("gen_priv_key your key {%x,%x}\n",pri.a,pri.b);
    return pri;
}
