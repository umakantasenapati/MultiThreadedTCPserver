#ifndef myThreadArgument_H
#define myThreadArgument_H

#include <string>

using namespace std;

class myTcpSocket;
const int MAX_NUM_CLIENTS = 10;

class myThreadArgument
{

private:
	

	//mySemaphore* coutSemaphore;       // semaphore for accessing the log file
	myTcpSocket* clientConnection;    // the connecting socket to the client
	std::string hostName;                // the name of the client
	int clienId;
	myThreadArgument* clientArgument[MAX_NUM_CLIENTS];
    int numOfConnectedClients;

	bool authStatus;
	bool signalToEnd;

public:

	
	myThreadArgument(myTcpSocket* client,int id);
	~myThreadArgument() ;

	// use the accessor functions that fit your needs

	//mySemaphore* getCoutSemaphore()     { return coutSemaphore; }
	myTcpSocket* getClientConnect()     { return clientConnection; }
	std::string&      getHostName()          { return hostName; }
	bool getSignalToEnd()               { return signalToEnd; }
	void setSignalToEnd(bool s)         { signalToEnd = s; }
	bool getAuthStatus()               { return authStatus; }
	void setAuthStatus(bool s)         { authStatus = s; }
	int getClientId() {return clienId; }

	void addClientArgument(myThreadArgument*);
	myThreadArgument* getClientArgument(int);

	
};

#endif