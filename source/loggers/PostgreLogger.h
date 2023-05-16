/*
 * PostgreLogger.h
 *  This is a thread safe logger.
 *      Author: pasquale
 */

#ifndef POSTGRELOGGER_H_
#define POSTGRELOGGER_H_

#include <stdexcept>
#include <thread>
#include <cstring>
#include "../postgresql/libpq-fe.h"
#include "../loggers/PGcommand.h"
#include "../fifo/CircularBuffer.h"

//#include <mutex>  // alredy included in circularBuffer
//#include <iostream> // for debugging

class PostgreLogger: public CircularBuffer {

private:

	using CircularBuffer::poll;
	using CircularBuffer::peek;





protected:
	PGconn *conn;
	bool stopFlag;
	bool isLooping;

	virtual void logOnPostGres()
	{
		isLooping=true;

		//std::cout<<"PostgreLogger::logOnPostGres iniziato"<<std::endl;
		ExecStatusType est;
		char dest[sizeof(PGcommand)];

		while(stopFlag!=true){

			if(poll(dest, sizeof(PGcommand)) ==false){
				std::this_thread::yield();
			}else{
				PGcommand *pgcom=static_cast<PGcommand*>(static_cast<void*>(dest) );
                const char* values[30]; for(int i=0;i<pgcom->paramNum;i++){values[i]=pgcom->paramValues[i].data();}
                //char **tmp=new char*[pgcom->paramNum]; for (int i=0;i<pgcom->paramNum;i++){tmp[i]=new char[30];std::strcpy(tmp[i],pgcom->paramValues[i].c_str()); values[i]=tmp[i];}
                //for(int i=0;i<pgcom->paramNum;i++){pgcom->paramValues[i].copy(values[i],pgcom->paramValues[i].size());values[i][pgcom->paramValues[i].size()+1]='\0';}

                PGresult *res=PQexecParams(conn, pgcom->command , pgcom->paramNum, pgcom->oids, values, NULL, NULL, 0);

                if (/*ExecStatusType*/ (est=PQresultStatus(res) )!= PGRES_COMMAND_OK)
				{
				   	std::cerr<<"Insert failed: "<<PQresStatus(est)<<std::endl;
				   	pgcom->printToStdOut();
				}
				//memset(dest,0,sizeof(PGcommand));
			}
		}
		isLooping=false;
		std::cout<<"PostgreLogger::logOnPostGres finito"<<std::endl;
	}



public:
	PostgreLogger(unsigned int fifoSize,PGconn &conn)
	: CircularBuffer(fifoSize),conn(&conn),isLooping(false)
	{
		//prodFlag=false;
		//consFlag=false;
		//turno=0;//1 per producer 2 per consumer
		stopFlag=false;


	}

	PostgreLogger(unsigned int fifoSize, const char * keywords[],const char * values[])
	: CircularBuffer(fifoSize),isLooping(false)
	{
		//prodFlag=false;
		//consFlag=false;
		//turno=0;//1 per producer 2 per consumer
		stopFlag=false;

		conn=PQconnectdbParams(keywords, values, 1);
        char *feedback;
        bool flag=true;
        while(flag==true){
            switch(PQstatus(conn))
            {
                case CONNECTION_BAD:
        			throw std::runtime_error(PQerrorMessage(conn));
                    break;

                case CONNECTION_STARTED:
                    feedback = "Connecting...";
                    break;

                case CONNECTION_MADE:
                    feedback = "Connected to server...";
                    break;
                case CONNECTION_OK:
                    feedback = "Succesfully connected to postgresDBMS";
                    flag=false;
                    break;
                default:
                    feedback = "Connecting...";
            }
            std::cout<<feedback<<std::endl;
        }
//		if (PQstatus(this->conn)!=CONNECTION_OK){
//			throw std::runtime_error("PostgreLogger: connection with postgres dbms failed!");
//		}

	}


	virtual ~PostgreLogger(){};

	virtual void startLogLoop()
	{
		if(isLooping) return;
		stopFlag=false;
		std::thread t(&PostgreLogger::logOnPostGres,this);
		t.detach();
	}

	virtual void stopLogLoop(){
		stopFlag=true;
	}

};

#endif /* POSTGRELOGGER_H_ */
