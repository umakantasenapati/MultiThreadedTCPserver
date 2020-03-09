#include "myThreadArgument.h"

myThreadArgument::myThreadArgument(shared_ptr<myTcpSocket> client,int id,int noOfClients)
{
	cout<<"******Inside myThreadArgument constructor********"<<'\n';
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
	cout<<"*******Inside myThreadArgument destructor********"<<'\n';
}

void myThreadArgument::addClientArgument(shared_ptr<myThreadArgument> argument)
{
	if ( argument )
		clientArgument[numOfConnectedClients++] = argument;
}

shared_ptr<myThreadArgument> myThreadArgument::getClientArgument(int index)
{
	if ( index < MAX_NUM_CLIENTS ) return clientArgument[index];
	return NULL;
}


