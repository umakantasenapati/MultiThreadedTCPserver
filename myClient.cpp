#include "mySocket.h"
#include "myLog.h"
#include "myException.h"
#include "myHostInfo.h"

myLog LOG;
void readServerConfig(string&);
void checkFileExistence(const string&);

int main()
{
	// Initialize the winsock library
	myTcpSocket::initialize();
        
	// get client's information (assume neither the name nor the address is given)
	LOG << endl;
	LOG << "Retrieve the localHost [CLIENT] name and address:" << endl;
    myHostInfo clientInfo;
	string clientName = clientInfo.getHostName();
    string clientIPAddress = clientInfo.getHostIPAddress();
    cout << "Name: " << clientName << endl;
    cout << "Address: " << clientIPAddress << endl;
	LOG << "		==> Name: " << clientName << endl;
	LOG << "		==> Address: " << clientIPAddress << endl;

	// get server's IP address and name
	string serverIPAddress = "";
	readServerConfig(serverIPAddress);
	LOG << endl;
	LOG << "Retrieve the remoteHost [SERVER] name and address:" << endl;
	LOG << "		==> the given address is " << serverIPAddress << endl;

    myHostInfo serverInfo(serverIPAddress,ADDRESS);
	string serverName = serverInfo.getHostName();
    cout << "Name: " << serverName << endl;
    cout << "Address: " << serverIPAddress << endl;
	LOG << "		==> Name: " << serverName << endl;
	LOG << "		==> Address: " << serverIPAddress << endl;

    // create the socket for client
    myTcpSocket myClient(PORTNUM);
	cout << myClient;
	LOG << "client configuation: " << endl;
	LOG << myClient;

    // connect to the server.
	cout   << "connecting to the server [" << serverName << "] ... " << endl;
	LOG << "connecting to the server [" << serverName << "] ... " << endl;
	myClient.connectToServer(serverIPAddress,ADDRESS);

	int recvBytes = 0;
	while (1)
	{
		// send message to server
		char messageToServer[MAX_MSG_LEN+1];
		memset(messageToServer,0,sizeof(messageToServer));
		cout << "[SEND] ";
		cin.getline(messageToServer,MAX_MSG_LEN);

		LOG << "[SEND] " << messageToServer << endl;
		myClient.sendMessage(string(messageToServer));
		if ( !string(messageToServer).compare("Quit") || !string(messageToServer).compare("quit") ) break;

		// receive message from server
		string messageFromServer = "";
        recvBytes = myClient.recieveMessage(messageFromServer);
		if ( recvBytes == -99 ) break;

		cout   << "[RECV:" << serverName << "]: " << messageFromServer << endl;
		LOG << "[RECV:" << serverName << "]: " << messageFromServer << endl;

	}

    return 1;
}

void readServerConfig(string& serverIPAddr)
{
	string serverConfigFile = "serverConfig.txt";	
	checkFileExistence(serverConfigFile);
	ifstream serverConfig(serverConfigFile.c_str());

	// read server's IP address
	getline(serverConfig,serverIPAddr);
	serverConfig.close();
}

void checkFileExistence(const string& fileName)
{
	ifstream file(fileName.c_str());
	if (!file) 
	{
		cout << "Cannot continue:" << fileName << " does NOT exist!" << endl;
		exit(1);
	}
	file.close();
}