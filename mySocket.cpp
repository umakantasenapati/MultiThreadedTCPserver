#include "mySocket.h"
#include "myException.h"
#include "myLog.h"
#include <string>

const int MSG_HEADER_LEN = 6;

mySocket::mySocket(int pNumber)
{
    portNumber = pNumber;
    blocking = 1;

    try
    {
		if ( (socketId=socket(AF_INET,SOCK_STREAM,0)) == -1)
        {
				myException openSocketException(0,"error getting host by name");
				throw openSocketException;
			
        }
	}
    catch(myException& excp)
	{
		excp.response();
		exit(1);
	}

    
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_port = htons(portNumber);
}
    



ostream& operator<<(ostream& io,mySocket& s)
{
	string flagStr = "";

	io << endl;
	io << "Summary of socket settings:" << endl;
	io << "   Socket Id:     " << s.getSocketId() << endl;
	io << "   PORT NO :      "<<s.getPortNumber()<<endl;
	io << endl;
	return io;
}

myTcpSocket::~myTcpSocket()
{

}

void myTcpSocket::initialize()
{

}

void myTcpSocket::bindSocket()
{
	try
	{
		if (bind(socketId,(struct sockaddr *)&clientAddr,sizeof(struct sockaddr_in))==-1)
		{
			
				myException socketBindException(0,"error calling bind()");
				throw socketBindException;
			
        }
	}
    catch(myException& excp)
	{
		excp.response();
		exit(1);
	}
}


void myTcpSocket::connectToServer(string& serverNameOrAddr,hostType hType)
{ 
	

	myHostInfo serverInfo(serverNameOrAddr,hType);
	
    // Store the IP address and socket port number	
	struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverInfo.getHostIPAddress());
    serverAddress.sin_port = htons(portNumber);

    // Connect to the given address
	try 
	{
		if (connect(socketId,(struct sockaddr *)&serverAddress,sizeof(serverAddress)) == -1)
		{
			
				myException socketConnectException(0,"error calling connect()");
				throw socketConnectException;
			
        }
	}
    catch(myException& excp)
	{
		excp.response();
		exit(1);
	}
}

myTcpSocket* myTcpSocket::acceptClient(string& clientHost)
{
	int newSocket; 

    
    sockln_t clientAddressLen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientAddress;   

    // Accepts a new client connection and stores its socket file descriptor
	try 
	{
		if ((newSocket = accept(socketId, (struct sockaddr *)&clientAddress,&clientAddressLen)) == -1)
		{
			
				myException socketAcceptException(0,"error calling accept()");
				throw socketAcceptException;
			
        }
	}
    catch(myException& excp)
	{
		excp.response();
		return NULL;
	}
    
	// Get the host name given the address
    char *sAddress = inet_ntoa((struct in_addr)clientAddress.sin_addr);
	myHostInfo clientInfo(string(sAddress),ADDRESS);
	char* hostName = clientInfo.getHostName();
    clientHost += string(hostName);
	
    
    myTcpSocket* retSocket = new myTcpSocket();
	retSocket->setSocketId(newSocket);
    return retSocket;
}

void myTcpSocket::listenToClient(int totalNumPorts)
{
	try 
	{
		if (listen(socketId,totalNumPorts) == -1)
		{
			
				myException socketListenException(0,"error calling listen()");
				throw socketListenException;
			
        }
	}
    catch(myException& excp)
	{
		excp.response();
		exit(1);
	}
}       


int myTcpSocket::sendMessage(string message)
{
	int numBytes;  

	// Sends the message to host
	try 
	{
		if (numBytes = send(socketId,message.c_str(),message.size(),0) == -1)
		{
			
				myException socketSendException(0," error calling send()");
				throw socketSendException;
			
        }
	}
    catch(myException& excp)
	{
		excp.response();
		exit(1);
	}

	return numBytes;
}


int myTcpSocket::recieveMessage(char* message)
{
	int numBytes; 

	
	// receive the  message
	try
	{
		numBytes = recv(socketId,message,MAX_MSG_LEN,0);
        if (numBytes == -1)
        {
			myException socketRecvException(0,"error in calling recv()");
			throw socketRecvException;
		}
	}
    catch(myException& excp)
	{
		excp.response();
		exit(1);
	}

    return numBytes;
}


