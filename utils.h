

struct program_arguments
{
    bool generate_keys;
    bool encode_data;
    bool decode_data;
    bool key_file_present;
    bool data_file_present;
    char key_file[256]; /** path to key file **/
    char data_file[256]; /** path to data file **/
    program_arguments();
};

struct key_file_struct
{
    unsigned long key1;		/** public key **/	
    unsigned long key2;		/** private key **/

    key_file_struct();

    /** build structure from data stream **/
    void set(char*data);
    /** build data stream from structure **/
    char* get();

};


