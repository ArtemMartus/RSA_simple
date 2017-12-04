

class Exception
{
    protected:
    const char* code;
    const char* description;
    char* error_data;
    public:
    Exception(const char* code,const char* description);
    ~Exception();
    char* what();
};

class IOException: public Exception
{
    public:
	IOException(const char* code, const char* description);
};

class ArgumentException: public Exception
{
    public:
	ArgumentException(const char* code,const char* description);
};
