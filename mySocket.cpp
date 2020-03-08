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
				myException openUnixSocketException(0,"error getting host by name");
				throw openUnixSocketException;
			
        }
	}
    catch(myException& excp)
	{
		excp.response();
		exit(1);
	}

    /* 
	   set the initial address of client that shall be communicated with to 
	   any address as long as they are using the same port number. 
	   The clientAddr structure is used in the future for storing the actual
	   address of client applications with which communication is going
	   to start
	*/
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
			
				myException unixSocketBindException(0,"error calling bind()");
				throw unixSocketBindException;
			
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
	/* 
	   when this method is called, a client socket has been built already,
	   so we have the socketId and portNumber ready.

     */

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
			
				myException unixSocketConnectException(0,"error calling connect()");
				throw unixSocketConnectException;
			
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
	int newSocket;   // the new socket file descriptor returned by the accept systme call

    // the length of the client's address
    int clientAddressLen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientAddress;    // Address of the client that sent data

    // Accepts a new client connection and stores its socket file descriptor
	try 
	{
		if ((newSocket = accept(socketId, (struct sockaddr *)&clientAddress,&clientAddressLen)) == -1)
		{
			
				myException unixSocketAcceptException(0,"error calling accept()");
				throw unixSocketAcceptException;
			
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
	
    // Create and return the new myTcpSocket object
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
			
				myException unixSocketListenException(0,"error calling listen()");
				throw unixSocketListenException;
			
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
	int numBytes;  // the number of bytes sent

	// Sends the message to the connected host
	try 
	{
		if (numBytes = send(socketId,sendMsg.c_str(),sendMsg.size(),0) == -1)
		{
			
				myException unixSocketSendException(0," error calling send()");
				throw unixSocketSendException;
			
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
	int numBytes;  // The number of bytes recieved

	
	// receive the real message
	try
	{
		numBytes = recv(socketId,message,2000,0);
        if (numBytes == -1)
        {
			myException unixSocketRecvException(0,"error calling recv()");
			throw unixSocketRecvException;
		}
	}
    catch(myException& excp)
	{
		excp.response();
		exit(1);
	}

    return numBytes;
}


