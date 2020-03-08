#include "myMessage.h"

myMessage::myMessage()
{}

myMessage::myMessage(msgType type,int content):header(type),payload(content)
{
	
}

myMessage::myMessage(const myMessage& refMsg)::header(refMsg.header),payload(refMsg.payload)
{
	
}

msgType myMessage::getHeader()
{
	return header;
}

int myMessage::getPayload()
{
	return payload;
}
string  myMessage::messageToString()
{
	stringstream ss;
	ss<<(int)header<< ' '<<payload;
	string s=ss.str();
	return s;
	
}