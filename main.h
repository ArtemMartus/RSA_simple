void InitArguments(int argc,char**argv,program_arguments** /**out**/ args); /** initialize program_arguments structure **/
void ParseArguments(int argc,char**argv,program_arguments* args); /** parse programm arguments **/

void GenerateKeys(program_arguments* args);	/** keys generator **/
void EncodeData(program_arguments* args);	/** data encoder helper **/
void DecodeData(program_arguments* args);	/** data decoder helper **/


