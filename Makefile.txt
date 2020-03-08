Server: SeverForMultipleClient.cpp myThreadArgument.cpp mySocket.cpp myMessage.cpp myLog.cpp myHostInfo.cpp myException.cpp 
	g++ -o Server SeverForMultipleClient.cpp myThreadArgument.cpp mySocket.cpp myMessage.cpp myLog.cpp myHostInfo.cpp myException.cpp  -lpthread -std=c++11

Client: myClient.cpp  mySocket.cpp myMessage.cpp myLog.cpp myHostInfo.cpp myException.cpp 
	g++ -o Client myClient.cpp mySocket.cpp myMessage.cpp myLog.cpp myHostInfo.cpp myException.cpp  -lpthread -std=c++11

all:Server Client
clean:
	rm -rf Server Client