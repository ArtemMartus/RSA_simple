#include <iostream>
using namespace std;

int main(int argc,char**argv)
{
    if(argc<3)
	return -1;
    int a =0;
    int b =0;
    sscanf(argv[1],"%x",&a);
    sscanf(argv[2],"%x",&b);
    cout<<"You've entered two numbers:"<<a<<":"<<b<<endl;
    int i =255%abs(a>b?b:a);
    cout<<"i = "<<i<<endl;
    int iterations = 0;
    while(true)
    {
	++iterations;
	if(i==0)
	{
	    cout<<"bad input!"<<endl;
	    return -1;
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
    cout<<"Finished with "<<iterations<<" iterations..."<<endl;
    cout<<"Your both simple numbers are "<<a<<":"<<b<<endl;
    return 0;
}
