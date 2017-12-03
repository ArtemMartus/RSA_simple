#include <iostream>
#include "main.h"
#include "rsa.h"
#include "excep.h"
using namespace std;


/** TODO:
  1) Finish encoding\decoding functions
  2) Write encoding\decoding\generate\parse function realization into separate files
  3) Write helping data & function names into main.h
  4) Write rsa.(h\cpp)
  5) Write excep.(h\cpp)
 **/

const unsigned int max_path_size = 256;
const unsigned int max_data_size_stdin = 1024*8;

bool generate_keys = false;
bool encode_data = false;
bool decode_data = false;
bool key_file_present = false;
bool data_file_present = false;
char key_file[max_path_size]={0}; /** path to key file **/
char data_file[max_path_size]={0}; /** path to data file **/

struct key_file_struct
{
    int file_tag;	/** unique key_file_struct file starting tag **/
    char* key1;		/** public key **/
    int key1_size;	
    char*key2;		/** private key **/
    int key2_size;

    key_file_struct(){
	file_tag = 0 | 1<<5 | 1 << 10 | 1<< 15 | 1<< 20| 1 << 21;
	key1_size=key2_size = 0;
	key1 = key2 = 0;
    }
    ~key_file_struct()
    {
	if(key1!=0)
	    delete[]key1;
	if(key2!=0)
	    delete[]key2;
    }
    /** build structure from data stream **/
    void set(char*data)
    {
	sscanf(data,"%d\r%d={%s}\r%d={%s}\r",&file_tag,key1,&key1_size,key2,&key2_size);
    }
    /** build data stream from structure **/
    char* get()
    {
	size_t needed_memory = sprintf(0,"%d\r%d={%s}\r%d={%s}\r",file_tag,key1_size,(key1==0?"":key1),key2_size,(key2==0?"null":key2))+sizeof('\0');
	char* data = new char[needed_memory];
	snprintf(needed_memory,data,"%d\r%d={%s}\r%d={%s}\r",file_tag,key1_size,(key1==0?"":key1),key2_size,(key2==0?"null":key2));
	return data; /** do not forget to delete[]data !!! **/
    }

};

void ParseArguments(int argc,char**argv); /** parse programm arguments **/

void GenerateKeys();	/** keys generator **/
void EncodeData();	/** data encoder helper **/
void DecodeData();	/** data decoder helper **/

/** main function **/
int main(int argc,char**argv)
{
    /** surround it with try{}catch(...){} to capture all exceptions **/
    try{
	ParseArguments(argc,argv);

	if(generate_keys)
	    GenerateKeys();
	else if(encode_data)
	    EncodeData();
	else if(decode_data)
	    DecodeData();

    }
    catch( Exception* e)
    {
	printf("Exception handler called!\nException details:%s\nQuitting...\n",e->what());
	delete e;
	exit(0);
    }

    printf("Thank you for using this awesome application!\nHave fun and keep your information safe with us!\n\n");
    return 0;
}

void ParseArguments(int argc,char**argv)
{
    if(argc==1)
	throw new ArgumentException(ARG_ABSENT,"Use --help to get help information.\n");

    /** Flick through all programm arguments passed except first one **/

    for(int i = 1;i<argc;++i)
    {
	char* arg  = argv[i];
	cout<<"##Argument "<<i<<" =\t\t"<<arg<<endl;
	/** Handle '-key' argument **/
	if(_stricmp(arg,"-key")==0)
	{
	    ++i; /** following argument MUST be path to key file **/
	    if(i >= argc)
		throw new ArgumentException(ARG_BADINPUT,"no key specified after -key command.");
	    strncpy(key_file,argv[i],sizeof(key_file)-1);

	    /** check whether we can read file **/
	    FILE* fo = fopen(key_file,"r");
	    if(fo==NULL)
		fo = fopen(key_file,"w"), if(fo==NULL)
		    throw new IOException(ARG_BADFILE,"cannot open/create key file.");
	    fclose(fo);
	    key_file_present = true;
	    continue;
	}
	/** Handle -gen(erate keys) argument **/
	if(_stricmp(arg,"-gen")==0)
	{
	    generate_keys = true;
	    continue;
	}
	/** Handle -encode file argument **/
	if(_stricmp(arg,"-encode")==0)
	{
	    encode_data = true;
	    continue;
	}
	/** Handle -decode file argument **/
	if(_stricmp(arg,"-decode")==0)
	{
	    decode_data = true;
	    continue;
	}
	/** Handle -data argument **/
	if(_stricmp(arg,"-data")==0)
	{
	    /** following argument MUST be path to file on which data we need to make some work **/ 
	    ++i;
	    if(i>=argc)
		throw new ArgumentException(ARG_BADINPUT,"no data file specified after -data command.");
	    strncpy(data_file,argv[i],sizeof(data_file)-1);
	    FILE* fo = fopen(data_file,"r");
	    if(fo==NULL)
		fo = fopen(data_file,"w"),if(fo==NULL)
		    throw new IOException(ARG_BADFILE,"cannot open/create data file.");
	    fclose(fo);
	    data_file_present = true;
	    continue;
	}
    }
}

void GenerateKeys()
{

    int ret = 0;
    FILE*f=0;
    /** check key file **/
    if(!key_file_present){
	bzero(key_file,sizeof(key_file));
	strncpy(key_file,"keyfile.rkey",sizeof(key_file)-1);
    }

    f = fopen(key_file,"w");
    if(f==0)
	throw new IOException(KEYFILE_IO_ERROR,"cannot open keyfile for writing.");

    /** get main two numbers to generate keys **/
    long a = 0;
    long b = 0;
    printf("Enter A = ");
    scanf("%d",&a);

    printf("Enter B = ");
    scanf("%d",&b);

    /** generate and write keys to file **/
    key_file_struct kFile = key_file_struct();

    int pub_key = RSA_gen_public_key(a,b);
    int pri_key = RSA_gen_private_key(a,b);

    int key1_size = sprintf(0,"%d",pub_key)+sizeof('\0');
    int key2_size = sprintf(0,"%d",pri_key)+sizeof('\0');

    char* key1 = new char[key1_size];
    snprintf(key1_size,key1,"%d",pub_key);

    char* key2 = new char[key2_size];
    snprintf(key2_size,key2,"%d",pri_key);

    /** write file by some signature **/

    kFile.key1 = key1;
    kFile.key2 = key2;
    kFile.key1_size = key1_size;
    kFile.key2_size = key2_size;

    char* data = kFile.get();
    ret = fwrite((void*)data,strlen(data)+sizeof('\0'),1,f);
    delete[]data;
    fclose(f);
    if(ret==0)
	throw new IOException(KEYFILE_IO_ERROR,"cannot write data block.");
    printf("Successfully written keys to %s!\n",key_file);
}

void EncodeData()
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

    if(!key_file_present)
    {
	/** If we do not have key file currently - ask for public key from stdin **/
    }
    else
    {
	/** Rebuild key from key file **/
    }

    if(data_file_present)
    {
	FILE * f = 0;
	int ret = 0;
	int file_size = 0;

	f = fopen(data_file,"r");
	if(f == 0)
	    throw new IOException(DATA_IO_ERROR,"cannot open read stream from data file.");
	/** Obtain file size **/
	fseek(f,0,SEEK_END);
	file_size = ftell(f);
	data_size = file_size;
	rewind(f);
	printf("Woring on %s. %d bytes to process...\n",data_file,file_size);

	/** Load data file into memory **/
	data = new char[file_size];
	ret = fread((void*)data,1,file_size,f);	

	if(ret == 0)
	    throw new IOException(DATA_IO_ERROR,"cannot read data from file to encode.");
	fclose(f);
	printf("Successfully loaded data into memory!\n");
    }
    else
    {
	data = new char[max_data_size_stdin];
	bzero(data,max_data_size_stdin);

	printf("You have not specified data file!\nEnter data to encode(8kB maximum):");
	fgets(data,max_data_size_stdin,stdin);
	data[max_data_size_stdin-1]='\0';
    }
    printf("Encoding...\n");

    int encoded_data_size = 0;
    char* encoded_data = RSA_encode_data(data,key,&encoded_data_size);

    printf("Encoded data:\n========================================\n\n%s\n\n========================================\n",encoded_data);

    /** Now try to write it to the file **/

    FILE * f_out = 0;
    int ret = 0;
    char enc_data_file[max_path_size]={0};

    snprintf(sizeof(enc_data_file),enc_data_file,"%s_encoded",data_file);
    f_out = fopen(enc_data_file,"w");
    if(f_out == 0)
	throw new IOException(DATA_IO_ERROR,"cannot open write stream for encrypted data file.");
    ret = fwrite((void*)encoded_data,1,encoded_data_size,f_out);
    if(ret == 0)
	throw new IOException(DATA_IO_ERROR,"cannot write encrypted data block to a file.");
    fclose(f_out);

    delete [] data;
    delete [] encoded_data;

    printf("Successfully encoded data and wrote it to %s!\n",enc_data_file);
}

void DecodeData()
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

    if(!key_file_present)
    {
	/** If we do not have key file currently - ask for private key from stdin **/
    }
    else
    {
	/** Rebuild key from key file **/
    }

    if(data_file_present)
    {
	FILE * f = 0;
	int ret = 0;
	int file_size = 0;

	f = fopen(data_file,"r");
	if(f == 0)
	    throw new IOException(DATA_IO_ERROR,"cannot open read stream from data file.");
	/** Obtain file size **/
	fseek(f,0,SEEK_END);
	file_size = ftell(f);
	data_size = file_size;
	rewind(f);
	printf("Woring on %s. %d bytes to process...\n",data_file,file_size);

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
	data = new char[max_data_size_stdin];
	bzero(data,max_data_size_stdin);

	printf("You have not specified data file!\nEnter data to decode(8kB maximum):");
	fgets(data,max_data_size_stdin,stdin);
	data[max_data_size_stdin-1]='\0';
    }
    printf("Decoding...\n");

    int decoded_data_size = 0;
    char* decoded_data = RSA_decode_data(data,key,&decoded_data_size);

    printf("Decoded data:\n========================================\n\n%s\n\n========================================\n",decoded_data);

    /** Now try to write it to the file **/

    FILE * f_out = 0;
    int ret = 0;
    char dec_data_file[max_path_size]={0};

    snprintf(sizeof(dec_data_file),dec_data_file,"%s_decoded",data_file);
    f_out = fopen(dec_data_file,"w");
    if(f_out == 0)
	throw new IOException(DATA_IO_ERROR,"cannot open write stream for encrypted data file.");
    ret = fwrite((void*)decoded_data,1,decoded_data_size,f_out);
    if(ret == 0)
	throw new IOException(DATA_IO_ERROR,"cannot write encrypted data block to a file.");
    fclose(f_out);

    delete [] data;
    delete [] decoded_data;

    printf("Successfully decoded data and wrote it to %s!\n",dec_data_file);
}
