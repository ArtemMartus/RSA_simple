

struct program_arguments
{
    bool generate_keys;
    bool encode_data;
    bool decode_data;
    bool key_file_present;
    bool data_file_present;
    char key_file[max_path_size]; /** path to key file **/
    char data_file[max_path_size]; /** path to data file **/
    program_arguments();
};

struct key_file_struct
{
    int key1;		/** public key **/	
    int key2;		/** private key **/

    key_file_struct();

    /** build structure from data stream **/
    void set(char*data);
    /** build data stream from structure **/
    char* get();

};


