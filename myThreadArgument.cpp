#include "myThreadArgument.h"

myThreadArgument::myThreadArgument(myTcpSocket* client,int id,int noOfClients)
{
	
	clientConnection = client;
	signalToEnd = false;

	for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ ) clientArgument[i] = NULL;
    numOfConnectedClients = 0;
	authStatus=false;
	clienId=id;
	noOfAllowedClients=noOfClients;
	
}

myThreadArgument::~myThreadArgument() 
{
	
}

void myThreadArgument::addClientArgument(myThreadArgument* argument)
{
	if ( argument )
		clientArgument[numOfConnectedClients++] = argument;
}

myThreadArgument* myThreadArgument::getClientArgument(int index)
{
	if ( index < MAX_NUM_CLIENTS ) return clientArgument[index];
	return NULL;
}


