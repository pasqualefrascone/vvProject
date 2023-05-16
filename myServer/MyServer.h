/*
 * MyServer.h
 *
 *      Author: pasquale
 */

#ifndef UTILS_MYSERVER_H_
#define UTILS_MYSERVER_H_

#include "MyClientHandler.h"

class MyServer: public TCPServer {
private :
	PostgreLogger *pgLogger;
    std::string serverName;

public:
	MyServer(std::string serverName,int pendingConnectionQueueSize,int localPort,unsigned int buffReaderSize,unsigned int buffWriterSize)
	:TCPServer(pendingConnectionQueueSize, localPort, buffReaderSize, buffWriterSize),pgLogger(NULL),serverName(serverName) {}

	MyServer(std::string serverName,int pendingConnectionQueueSize,int localPort,unsigned int buffReaderSize,unsigned int buffWriterSize,PostgreLogger *pgLogger)
	:TCPServer(pendingConnectionQueueSize, localPort, buffReaderSize, buffWriterSize),pgLogger(pgLogger),serverName(serverName) {}


	MyServer(int localPort) 	:TCPServer(localPort),pgLogger(NULL),serverName(){}
	MyServer(int localPort,PostgreLogger *pgLogger) :TCPServer(localPort),pgLogger(pgLogger),serverName(){}


	virtual ~MyServer(){if(pgLogger!=NULL)delete pgLogger;};
	virtual ClientHandler* getClientHandler(/*ClientHandler *dest,*/BufferedSocket *bs) override{
		return new MyClientHandler(&serverName,bs,pgLogger);
		//return MyClientHandler(bs);
	}

};

#endif /* UTILS_MYSERVER_H_ */
