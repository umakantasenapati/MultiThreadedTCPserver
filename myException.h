#ifndef myException_H
#define myException_H

#include <string>
#include <iostream>
using namespace std;

class myException
{

public:

    // int: error code, string is the concrete error message
	myException(int,const string&);   
	~myException() {};

	
	virtual void response();  
	int getErrCode()    { return errorCode; }
	string& getErrMsg() { return errorMsg; }

private:
	void initVars();

private:
	int   errorCode;
	string errorMsg;
};

#endif