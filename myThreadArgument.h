#ifndef myThreadArgument_H
#define myThreadArgument_H

#include <string>
#include <memory>

using namespace std;

class myTcpSocket;
const int MAX_NUM_CLIENTS = 10;

class myThreadArgument
{

private:
	

	
	shared_ptr<myTcpSocket> clientConnection;    // the connecting socket to the client
	std::string hostName;                // the name of the client
	int clienId;
	shared_ptr<myThreadArgument> clientArgument[MAX_NUM_CLIENTS];
    int numOfConnectedClients;
	int noOfAllowedClients;

	bool authStatus;
	bool signalToEnd;

public:

	
	myThreadArgument(shared_ptr<myTcpSocket> client,int id,int noOfAllowedClients=0);
	~myThreadArgument() ;

	

	
	shared_ptr<myTcpSocket> getClientConnect()     { return clientConnection; }
	std::string&      getHostName()          { return hostName; }
	bool getSignalToEnd()               { return signalToEnd; }
	void setSignalToEnd(bool s)         { signalToEnd = s; }
	bool getAuthStatus()               { return authStatus; }
	void setAuthStatus(bool s)         { authStatus = s; }
	int getClientId() {return clienId; }
	int getNoOfAllowedClients() {return noOfAllowedClients;}

	void addClientArgument(shared_ptr<myThreadArgument>);
	shared_ptr<myThreadArgument> getClientArgument(int);

	
};

#endif