#ifndef mySocket_H
#define mySocket_H

#include "myHostInfo.h"

#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <sys/ioctl.h>
#include <memory>



const int MAX_RECV_LEN = 8096;
const int MAX_MSG_LEN = 1024;
const int PORTNUM = 8567;

class mySocket
{

protected:

	
	mySocket() {}
	void setSocketId(int socketFd) { socketId = socketFd; }

protected:

    int portNumber;        // Socket port number    
    int socketId;          // Socket file descriptor

    int blocking;          // Blocking flag
    int bindFlag;          // Binding flag

    struct sockaddr_in clientAddr;    

public:

    mySocket(int);                       

    virtual ~mySocket()
    {
		
            close(socketId);
        
     }

public:

	

    // size of the send and receive buffer

    void setSendBufSize(int);
    void setReceiveBufSize(int);

    // retrieve socket option settings

    
	
    // returns the socket file descriptor
    int getSocketId() { return socketId; }

	// returns the port number
	int getPortNumber() { return portNumber; }

	// show the socket 
	friend ostream& operator<<(ostream&,mySocket&);

private:

	
		char *sGetError()
		{
           return strerror(errno);
        }
	
};

class myTcpSocket : public mySocket
{



public:

	// initialization work, the clean up is in the destructor
	static void initialize();

public:

	/* 
	   Constructor. used for creating instances dedicated to client 
	   communication:

	   when accept() is successful, a socketId is generated and returned
	   this socket id is then used to build a new socket using the following
	   constructor, therefore, the next necessary call should be setSocketId()
	   using this newly generated socket fd
	*/
	myTcpSocket() {};
	~myTcpSocket();

	// Constructor.  Used to create a new TCP socket given a port
	myTcpSocket(int portId) : mySocket(portId) { };

	/*
	   Sends a message to a connected host. The number of bytes sent is returned
	   can be either server call or client call
	*/
	int sendMessage(string);

	/*
	   receive messages and stores the message in a buffer
	*/
	int recieveMessage(char *);

	/*
	   Binds the socket to an address and port number
	   a server call
	*/
	void bindSocket();

	/*
	   accepts a connecting client.  The address of the connected client 
	   is stored in the parameter
	   a server call
	*/
	shared_ptr<myTcpSocket> acceptClient(string&);

	// Listens to connecting clients, a server call
	void listenToClient(int numPorts = 5);

	// connect to the server, a client call
	virtual void connectToServer(string&,hostType);



};

#endif
        
