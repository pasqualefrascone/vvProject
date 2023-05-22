/*
 * MyClient.h
 *
 *      Author: pasquale
 */

#ifndef MYCLIENT_H_
#define MYCLIENT_H_



#include "../source/connprotocol/Client.h"
#include "../source/loggers/PostgreLogger.h"
#include "../source/utils/PostGresOid.h"
using namespace std;
class MyClient: public Client {
private:
	PostgreLogger *pgLogger;
    std::string clientName;
public:
	MyClient(char* hostAddress,unsigned short hostPort, unsigned int sizeOfRbuff,unsigned int sizeOfWbuff,bool looping)
	:Client(hostAddress, hostPort, sizeOfRbuff, sizeOfWbuff, looping),pgLogger(NULL)
    {
        char hostname[256];
        if (gethostname(hostname, 256) == 0) {
            this->clientName =  std::string (hostname);
        } else {
            std::cerr << "Failed to get hostname." << std::endl;
            this->clientName =  std::string ("client");
        }
    }

	MyClient(char* hostAddress,unsigned short hostPort, unsigned int sizeOfRbuff,unsigned int sizeOfWbuff,bool looping,PostgreLogger *pgLogger)
	:Client(hostAddress, hostPort, sizeOfRbuff, sizeOfWbuff, looping),pgLogger(pgLogger)
    {
        char hostname[256];
        if (gethostname(hostname, 256) == 0) {
            this->clientName =  std::string (hostname);
        } else {
            std::cerr << "Failed to get hostname." << std::endl;
            this->clientName =  std::string ("client");
        }
    }


	virtual ~MyClient(){if(pgLogger!=NULL){delete pgLogger;} ;};

	virtual void ignoreLine(){
		    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	virtual void start(){
		//bs->stopReadLoop();bs->stopWriteLoop();
		//std::cout<<"Myclient: started..."<<std::endl;
		//int command=-1;
		int pc=0;
		int op1{},op2{},res{};
		char logmsg[40];




        PGcommand *pgcomm;
    	char timeStamp[27];//Util::getTimeStamp(timeStamp);

        bool flag=true;
		while(flag){
			switch (pc) {
				case 0:
					std::cout<<"inserisci operando 1: ";
					std::cin>>op1;
                    if(cin.eof()){
                        flag=false;
                        break;
                    }

					//std::cout<<std::endl;
					//if(std::cin.fail()){std::cin.clear();ignoreLine();std::cout<<"Illegal Input! :"<<std::cin.fail();<<std::endl;continue;}
					bs->addOnWriteBuffer(op1);
					pc=30;
					break;
				case 30:
					bs->writeSignal();
					//print("client: time = "+String(time)+"; pc="+String(pc)+"; op1: writefifodata[1]="+String(writefifodata[1])+"\n");
                    pc=1;
					if (pgLogger==NULL) break;
					Util::getTimeStamp(timeStamp);
					memset(logmsg,'\0',50);//Util::resetCharBuf(logmsg, sizeof(logmsg));
					snprintf(logmsg,50,"%s%d","write fifo data: ",op1);
					pgcomm=PGcommandBuilder().setParamNum(4)
							->setPGCommand("INSERT INTO agent.logs(who,whenn,pc,log) VALUES($1,$2,$3,$4);")
							->addParVal(clientName.data())->addParVal(timeStamp)->addParVal(std::to_string(pc).data())->addParVal(logmsg)
							->addOid(VARCHAR)->addOid(TIMESTAMP)-> addOid(INT2)->addOid(VARCHAR)
							->build();
					pgLogger->add(static_cast<char*>(static_cast<void*>(pgcomm)),sizeof(*pgcomm));


					break;
				case 1:
					std::cout<<"inserisci operando 2: ";
					std::cin>>op2;
                    if(cin.eof()){
                        flag=false;
                        break;
                    }

                    //std::cout<<std::endl;
					//if(std::cin.fail()){std::cin.clear();ignoreLine();std::cout<<"Illegal Input! "<<std::endl;continue;}

					bs->addOnWriteBuffer(op2);
					pc=40;
					break;
				case 40:
					bs->writeSignal();
                    pc=2;

                    //print("client: time = "+String(time)+"; pc="+String(pc)+"; op2: writefifodata[1]="+String(writefifodata[1])+"\n");
					if (pgLogger==NULL) break;
					Util::getTimeStamp(timeStamp);
					memset(logmsg,'\0',50);//Util::resetCharBuf(logmsg, sizeof(logmsg));
					snprintf(logmsg,50,"%s%d","write fifo data: ",op2) ;
					pgcomm=PGcommandBuilder().setParamNum(4)
								->setPGCommand("INSERT INTO agent.logs(who,whenn,pc,log) VALUES($1,$2,$3,$4);")
							->addParVal(clientName.data())->addParVal(timeStamp)->addParVal(std::to_string(pc).data())->addParVal(logmsg)
							->addOid(VARCHAR)->addOid(TIMESTAMP)-> addOid(INT2)->addOid(VARCHAR)
							->build();
					pgLogger->add(static_cast<char*>(static_cast<void*>(pgcomm)),sizeof(*pgcomm));

					break;
				case 2:
                    bs->readSignal();
                    pc=20;
                    break;
				case 20:
                    pc=0;
                    while(true){//waiting for server
						//std::cout<<"provo a pollare : "<<std::endl;
						if(!bs->pollFromReadBuffer(res)) std::this_thread::yield();//std::this_thread::sleep_for(std::chrono::milliseconds(1));
						else break;
					}
					std::cout<<"res is:" <<res<<std::endl;
					//print("client: time = "+String(time)+"; pc="+String(pc)+"; op1-op2 = value="+String(value)+"\n"); on postgre
					if (pgLogger==NULL) break;


					Util::getTimeStamp(timeStamp);

					//Util::resetCharBuf(logmsg, sizeof(logmsg));
					snprintf(logmsg,50,"%s%d","op1-op2: ",res);
					pgcomm=PGcommandBuilder().setParamNum(4)
								->setPGCommand("INSERT INTO agent.logs(who,whenn,pc,log) VALUES($1,$2,$3,$4);")
							->addParVal(clientName.data())->addParVal(timeStamp)->addParVal(std::to_string(pc).data())->addParVal(logmsg)
							->addOid(VARCHAR)->addOid(TIMESTAMP)-> addOid(INT2)->addOid(VARCHAR)
							->build();
					pgLogger->add(static_cast<char*>(static_cast<void*>(pgcomm)),sizeof(*pgcomm));

					break;

				default:
					break;
			}
		}

		if(pgLogger!=NULL)pgLogger->stopLogLoop();

	}

};

#endif /* MYCLIENT_H_ */
