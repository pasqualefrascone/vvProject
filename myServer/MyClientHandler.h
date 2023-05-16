/*
 * MyClientHandler.h
 *
 *      Author: pasquale
 */

#ifndef MYCLIENTHANDLER_H_
#define MYCLIENTHANDLER_H_

#include "../source/connprotocol/ClientHandler.h"



#include <iostream>
#include <thread>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <string>
#include "../source/loggers/PostgreLogger.h"
#include "../source/utils/Util.h"
#include "../source/utils/PostGresOid.h"

class MyClientHandler: public ClientHandler {
private:
    PostgreLogger *pgLogger;
    std::string *serverName;
public:

	MyClientHandler(std::string *serverName,BufferedSocket *sock):ClientHandler(sock),pgLogger(NULL),serverName(serverName)
    {
        if (serverName == nullptr || serverName->size() == 0) {
            char hostname[256];
            if (gethostname(hostname, 256) == 0) {
                this->serverName = new std::string (hostname);
            } else {
                std::cerr << "Failed to get hostname." << std::endl;
            }
        }
    }
	MyClientHandler(std::string *serverName,BufferedSocket *sock,PostgreLogger *pgLogger):ClientHandler(sock),pgLogger(pgLogger),serverName(serverName)
    {
        if (serverName == nullptr || serverName->size() == 0) {
            char hostname[256];
            if (gethostname(hostname, 256) == 0) {
                this->serverName = new std::string (hostname);
            } else {
                std::cerr << "Failed to get hostname." << std::endl;
            }
        }
    }

	virtual ~MyClientHandler(){}

	virtual void startHandle(){
		runningFlag=true;
		//std::cout<<"MyClientHandler: starting handling..."<<std::endl;

		std::thread t (&MyClientHandler::handle,this);
		t.detach();
	}

    virtual void handle()     {
    	//std::cout<<"MyClientHandler: handling..."<<std::endl;
    	PGcommand *pgcomm;
        std::string serverName(*(this->serverName));

    	    	//std::cout<<"MyClientHandler: handling..."<<std::endl;
    	    	/// INIZIO




    	    	// timestamp variable
    	    	char timeStamp[27];
    	    	//std::cout<<"Client:start (timeStamp): "<<timeStamp<<std::endl;


    	    	//log that client has been started
    	    	if(pgLogger!=NULL){
    	    		Util::getTimeStamp(timeStamp);
        	    	pgcomm=PGcommandBuilder().setParamNum(4)
    									->setPGCommand("INSERT INTO agent.logs(who,whenn,pc,log) VALUES($1,$2,$3,$4);")
                            ->addParVal(this->serverName->data())
                            ->addParVal(timeStamp)->addParVal(std::to_string(-1).data())
                            ->addParVal((serverName+std::string(" : ClientManager started")).data())
        	    			->addOid(VARCHAR)->addOid(TIMESTAMP)-> addOid(INT2)->addOid(VARCHAR)
        	    			->build();
        	    	//std::cout<<"pgcomm builded sizeof(pgcomm):"<<sizeof(*pgcomm)<<std::endl;
        	    	//pgcomm->printToStdOut();
        	    	pgLogger->add(static_cast<char*>(static_cast<void*>(pgcomm)),sizeof(*pgcomm));
        	    	//std::cout<<"pgcomm added"<<std::endl;
    	    	}


    	    	int pc=0,op1=0,op2=0,res=0;




    	    /* NON C'E' BISOGNO DI GESTIRE CONCORRENZA SICCOME ABBIAMO 2 FIFO, una per input e una per output
    	     * ed inoltre il socket è fullduplex
    	         //variabili peterson per gestione concorrenza su newest e oldest
    	    	 //bool in1 = false, in2 = false;
    	    	 //int turno;
    	    */
    	    	//std::thread tReceiver {[](Fifo fifoIn){fifoIn.listenOnFd();},std::ref(fifoIn)}; //cosi viene creata un'istanza copia di fifoIn. Non lo vogliamo.
    	    	//std::thread tSender {[](Fifo fifoOut){fifoOut.writeOnFd();},std::ref(fifoOut)};

    	    	//std::thread tSender (&Fifo::writeOnFd,std::ref(fifoOut));
    	    	//std::thread tReceiver (&Fifo::listenOnFd,std::ref(fifoIn));


    	    	while(stopFlag!=true){
    	    		switch (pc) {
    	    			case 0:
    	    				//readsignal
    	    				pc=10;
    	    				clientBSock->readSignal();

    	    				break;
    	    			case 10:
    	    				pc=1;
    	    				while(true){
    	    					bool fl=clientBSock->pollFromReadBuffer(op1);
    	    					if (fl==false) {
    	    						//std::this_thread::sleep_for(std::chrono::milliseconds(20));
    	    						continue;
    	    					}else {break;}
    	    				}
    	    				if(pgLogger==NULL) break;
    	    				Util::getTimeStamp(timeStamp);
    	    				//string logString="myServer: op1="+std::to_string(op1);
    	    				//print("myServer: time = "+String(time)+"; pc="+String(pc)+"; op1="+String(op1)+"\n");
    	        	    	pgcomm=PGcommandBuilder().setParamNum(4)
											->setPGCommand("INSERT INTO agent.logs(who,whenn,pc,log) VALUES($1,$2,$3,$4);")
                                    ->addParVal(serverName.data())
                                            ->addParVal(timeStamp)
                                            ->addParVal(std::to_string(pc).data())
									->addParVal( ( serverName+(" : op1="+std::to_string(op1))).data() )
    	        	    			->addOid(VARCHAR)->addOid(TIMESTAMP)-> addOid(INT2)->addOid(VARCHAR)
    	        	    			->build();
    	        	    	//std::cout<<"pgcomm builded sizeof(pgcomm):"<<sizeof(*pgcomm)<<std::endl;
    	        	    	//pgcomm->printToStdOut();
    	        	    	pgLogger->add(static_cast<char*>(static_cast<void*>(pgcomm)),sizeof(*pgcomm));


    	    				break;
    	    			case 1:
    	    				//readsignal
    	    				clientBSock->readSignal();
    	    				pc=20;

    	    				break;
    	    			case 20:
    	    				while(true){
    	    					if (!clientBSock->pollFromReadBuffer(op2)) {
    	    						//std::this_thread::sleep_for(std::chrono::milliseconds(20));
    	    						continue;
    	    					}else break;
    	    				}
    	    				//print("myServer: time = "+String(time)+"; pc="+String(pc)+"; op2="+String(op2)+"\n");
    	    				if(pgLogger==NULL) {pc=2;break;};

    	    				Util::getTimeStamp(timeStamp);
    	        	    	pgcomm=PGcommandBuilder().setParamNum(4)
											->setPGCommand("INSERT INTO agent.logs(who,whenn,pc,log) VALUES($1,$2,$3,$4);")
                                    ->addParVal(this->serverName->data())
                                    ->addParVal(timeStamp)
                                    ->addParVal(std::to_string(pc).data())
									->addParVal((serverName+(" : op2="+std::to_string(op2))).data() )
    	        	    			->addOid(VARCHAR)->addOid(TIMESTAMP)-> addOid(INT2)->addOid(VARCHAR)
    	        	    			->build();
    	        	    	//std::cout<<"pgcomm builded sizeof(pgcomm):"<<sizeof(*pgcomm)<<std::endl;
    	        	    	//pgcomm->printToStdOut();
    	        	    	pgLogger->add(static_cast<char*>(static_cast<void*>(pgcomm)),sizeof(*pgcomm));


    	    				pc=2;

    	    				break;
    	    			case 2:
    	    				//in fifo
    	    				pc=30;
    	    				res=op1-op2;
    	    				clientBSock->addOnWriteBuffer(res);

    	    				break;
    	    			case 30:
    	    				clientBSock->writeSignal();
    	    				if(pgLogger==NULL) {pc=0;break;}

    	    				Util::getTimeStamp(timeStamp);
    	        	    	pgcomm=PGcommandBuilder().setParamNum(4)
											->setPGCommand("INSERT INTO agent.logs(who,whenn,pc,log) VALUES($1,$2,$3,$4);")
                                    ->addParVal(this->serverName->data())
                                    ->addParVal(timeStamp)
                                    ->addParVal(std::to_string(pc).data())
									->addParVal( (serverName+(" : op1-op2=res"+std::to_string(res))).data() )
    	        	    			->addOid(VARCHAR)->addOid(TIMESTAMP)-> addOid(INT2)->addOid(VARCHAR)
    	        	    			->build();
    	        	    	//std::cout<<"pgcomm builded sizeof(pgcomm):"<<sizeof(*pgcomm)<<std::endl;
    	        	    	//pgcomm->printToStdOut();
    	        	    	pgLogger->add(static_cast<char*>(static_cast<void*>(pgcomm)),sizeof(*pgcomm));


    	    				pc=0;

    	    				//sendsignal
    	    				//log

    	    				break;

    	    			default:
    	    				break;
    	    		}
    	    	}



    	    	//htonl quando invio sempre
    	    	//ntohl quando ricevo


    }
};

#endif /* MYCLIENTHANDLER_H_ */
