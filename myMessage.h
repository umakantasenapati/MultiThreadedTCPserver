#ifndef myMessage_H
#define myMessage_H
#include <sstream>
#include <string>

using namespace std;

enum class msgType
{
	connReq=0,
	EOM,
	seqRequest,
	seqResp	
};

class myMessage
{
	msgType header;
	int payload;
	
	public:
	myMessage();
	myMessage(msgType,int);
	myMessage(const myMessage&);
	msgType getHeader();
	int getPayload();
	string messageToString();
	
};


#endif