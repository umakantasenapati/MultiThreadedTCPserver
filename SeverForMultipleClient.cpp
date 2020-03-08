#include "mySocket.h"
#include "myLog.h"
#include "myException.h"
#include "myHostInfo.h"
#include "myThreadArgument.h"
#inlcude "myMessage.h"
#include <thread>
#include <sstream>
#include <map>
#include <mutex>

std::map<int,bool> clientEOMStatusMap;
std::map<int,int> clientSeqNumMap;
std::mutex clientEOMStatusMtx;
std::muext clientSeqNumStatusMtx;

using namespace std;

myLog winLog;
void readServerConfig(string&);
void checkFileExistence(const string&);

void clientHandleThread(myThreadArgument* clientArgument)
{
	
	myTcpSocket* clientConnection = clientArgument->getClientConnect();
	int clientid = clientArgument->getClientId();

	// the server is communicating with this client here
	while(1)
	{
		string messageFromClient[2000];
		
		// receive from the client

        int numBytes = clientConnection->recieveMessage(messageFromClient);
		if ( numBytes == -99 ) break;

	
		stringstream ss;
		ss<<messageFromClient;
		int msgtype;
		int msgvalue;
		ss>>msgtype>>msgvalue;
		
		if(msgtype == (int)(msgType::connReq))
		{
			if(clientArgument->getAuthStatus()==false)
			{
				clientConnection->sendMessage(myMassage(msgType::seqRequest,10).messageToString);
			}
		}
		else if((msgtype != (int)(msgType::connReq)) && (clientArgument->getAuthStatus()==false) || (msgtype == (int)(msgType::EOM)))
		{
			
			lock_guard<std::mutex> guardEOMStatusMap(clientEOMStatusMtx);
			clientEOMStatusMap.insert(std::pair<int,bool>(clientid,true));
			clientArgument->setAuthStatus(false);
			break;			
		}
		else if(msgtype == (int)(msgType::seqResp))
		{
				lock_guard<std::mutex> guardClientSeqNumMap(clientSeqNumStatusMtx);
				clientSeqNumMap.insert(std::pair<int,int>(clientid,msgvalue));
				clientArgument->setAuthStatus(true);
		}
		
		cout   << endl << "RECV  from Client ID" << clientid << "msgtype"<<msgtype<<"msgvalue"<<msgvalue<<'\n';
		
		
    }

	
}

void serverHandleThread(myThreadArgument* serverArgument)
{
	// this structure will contain all the data this callback will work on
    //myThreadArgument* serverArgument = (myThreadArgument*)threadInfo;
		
	// the semamphore to protect the access to the std output
    //mySemaphore* coutSemaphore = serverArgument->getCoutSemaphore();

	// get the server
	myTcpSocket* myServer = serverArgument->getClientConnect();
	string serverName = serverArgument->getHostName();

	// bind the server to the socket
    myServer->bindSocket();
	cout   << endl << "server finishes binding process... " << endl;
	winLog << endl << "server finishes binding process... " << endl;
	
	// server starts to wait for client calls
	myServer->listenToClient();
	cout   << "server is waiting for client calls ... " << endl;
	winLog << "server is waiting for client calls ... " << endl;
	
	// server starts to listen, and generates a thread to 
	// handle each client

	myThreadArgument* clientArgument[MAX_NUM_CLIENTS];
	//myThread* clientHandle[MAX_NUM_CLIENTS];
	for ( int i = 0; i < MAX_NUM_CLIENTS; i++ )
	{
		clientArgument[i] = NULL;
		//clientHandle[i] = NULL;
	}
	int currNumOfClients = 0;
	int clientid=0;

	while ( 1 )
	{
		// wait to accept a client connection.  
		// processing is suspended until the client connects
    	myTcpSocket* client;    // connection dedicated for client communication
		string clientName;      // client name 
		client = myServer->acceptClient(clientName);	
		

		// lock the std out so we can write to the console
		//coutSemaphore->lock();
        cout   << endl << "==> A client from [" << clientName << "] is connected!" << endl << endl;
		winLog << endl << "==> A client from [" << clientName << "] is connected!" << endl << endl;
		//coutSemaphore->unlock();

		// for this client, generate a thread to handle it
		if ( currNumOfClients < MAX_NUM_CLIENTS-1 )
		{
			clientArgument[currNumOfClients] = new myThreadArgument(client,++clientid);
			//clientHandle[currNumOfClients] = new myThread(clientHandleThread,(void*)clientArgument[currNumOfClients]);
			std::thread clientThread(clientHandleThread,clientArgument[currNumOfClients]);
			clientThread.detach();
			
			serverArgument->addClientArgument(clientArgument[currNumOfClients]);
			currNumOfClients++;
		}
	}

    //return 1;
}

int main()
{
	
	winLog << endl;
	winLog << "Retrieve the local host name and address:" << endl;
	
    
	
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
	
	int serverid=1;
	

	// open socket on the local host(server) and show its configuration
	myTcpSocket myServer(PORTNUM);
	cout   << myServer;
	winLog << myServer;

	
	myThreadArgument* serverArgument = new myThreadArgument(&myServer,serverid);
	
	std::serverThread(serverHandleThread,serverArgument);
	serverThread.detach();
	
	

	// main control: since the serverThread is handling the server functions,
	// this main control is free to do other things.
	while ( 1 )
	{
		// do whatever you need to do here, I am using Sleep(x) 
		// to make a little delay, pretending to be the other 
		// possible processings.
		Sleep(50);

		// report the server status
		
		cout   << endl << "-----------------------------------------------------------------" << endl;
		winLog << endl << "-----------------------------------------------------------------" << endl;
		cout   << "server (name:" << serverName << ") status report:" << endl;
		winLog << "server (name:" << serverName << ") status report:" << endl;
		cout   << "   the following clients have successfully connected with server: " << endl;
		winLog << "   the following clients have successfully connected with server: " << endl;
		for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ )
		{
			myThreadArgument* clientInfo = serverArgument->getClientArgument(i);
			if ( clientInfo ) 
			{
				cout   << "         " << clientInfo->getHostName() << endl;
				winLog << "         " << clientInfo->getHostName() << endl;
			}
		}
		cout   << "   the following clients have shutdown the connection: " << endl;
		winLog << "   the following clients have shutdown the connection: " << endl;
		{
				lock_guard<std::mutex> guardEOMStatusMap(clientEOMStatusMtx);
				if(clientEOMStatusMap.size() ==MAX_NUM_CLIENTS)
				{
					for (auto it:clientEOMStatusMap)
					{
						cout<<"Client ID"<<it->first<<'\n';
					}
					break;
				}
		}
        cout   << "-----------------------------------------------------------------" << endl << endl;
		winLog << "-----------------------------------------------------------------" << endl << endl;
		//coutSemaphore.unlock();
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

