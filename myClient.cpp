#include "mySocket.h"
#include "myLog.h"
#include "myException.h"
#include "myHostInfo.h"
#include "myMessage.h"
#include <chrono>
#include <thread>

myLog LOG;
void readServerConfig(string&);
void checkFileExistence(const string&);

int main()
{
	
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
		
		myClient.sendMessage(myMessage(msgType::connReq,10).messageToString);
		
		char messageFromServer[2000];
		recvBytes = myClient.recieveMessage(messageFromServer);
		if ( recvBytes == -99 ) break;
		
		if(msgtype == (int)(msgType::seqRequest))
		{
			for(auto i=0;i<100;i++)
			{
				myClient.sendMessage(myMessage(msgType::seqResp,501).messageToString);
				std::this_thread::sleep_for(std::chrono::miliseconds(100));
			}
		}
		myClient.sendMessage(myMessage(msgType::EOM,501).messageToString);
		break;

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