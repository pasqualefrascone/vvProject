
//#include <iostream>

#include "../source/loggers/PostgreLogger.h"
#include "MyClient.h"
#include <cstdlib>
#include <climits>
using namespace std;


int main(int argc,char* argv[]) {

    if (argc != 3) {
        std::cout << "Usage: ./client serverHostname port" << std::endl;
        return 1;
    }

    char* serverHostname = argv[1];
    char* portStr = argv[2];

    unsigned long int portUL = 0;
    char* endPtr;
    portUL = std::strtoul(portStr, &endPtr, 10);

    if (*endPtr != '\0' || portUL > USHRT_MAX) {
        std::cout << "Invalid port number. Usage: ./program hostname port" << std::endl;
        return 1;
    }

    unsigned short int serverPort = static_cast<unsigned short int>(portUL);
	// setting up log on postgres
	const char * keywords[]={"host","port","dbname","user","password",NULL};
	const char * values[]={"pdb","5432","sistema","client","password",NULL};

	PostgreLogger *pgLogger=new PostgreLogger(1024,keywords,values);
	pgLogger->startLogLoop();

	MyClient client (serverHostname, serverPort, 100,100,false,pgLogger);

	client.start();

	return 0;
}
