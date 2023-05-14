
//#include <iostream>

#include "../source/loggers/PostgreLogger.h"
#include "../source/connprotocol/TCPServer.h"
#include "MyServer.h"
using namespace std;

//#include <iostream>
//#include <stdio.h>
//#include <unistd.h>
//#include <netinet/in.h> //strutture dati socket: sockaddr_in...
//#include <sys/types.h>
//#include <sys/socket.h> //domini implementativi AF
//#include <thread>
//#include <stdlib.h> //snprintf
//#include <arpa/inet.h>
//#include <netdb.h>

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>

int main() {

//    const char* hostname = "pdb";  // Sostituisci con l'hostname desiderato
//    std::cout<<"cerco "<<hostname<<std::endl;
//    struct addrinfo hints, *res;
//
//    // Imposta le opzioni per la query di getaddrinfo
//    memset(&hints, 0, sizeof(hints));
//    hints.ai_family = AF_UNSPEC;  // Utilizza IPv4 o IPv6
//    hints.ai_socktype = SOCK_STREAM;  // Tipo di socket (TCP)
//
//    // Esegui la query di getaddrinfo
//    int result = getaddrinfo(hostname, nullptr, &hints, &res);
//    if (result != 0) {
//        std::cerr << "Errore nella query di getaddrinfo: " << gai_strerror(result) << std::endl;
//        return 1;
//    }
//
//    // Itera sugli indirizzi ottenuti
//    for (struct addrinfo* addr = res; addr != nullptr; addr = addr->ai_next) {
//        char host[NI_MAXHOST];
//
//        // Ottieni il nome host dall'indirizzo
//        result = getnameinfo(addr->ai_addr, addr->ai_addrlen, host, sizeof(host), nullptr, 0, NI_NUMERICHOST);
//        if (result != 0) {
//            std::cerr << "Errore nella conversione dell'indirizzo: " << gai_strerror(result) << std::endl;
//            continue;
//        }
//
//        // Stampa l'indirizzo IP
//        std::cout << "Indirizzo IP: " << host << std::endl;
//    }
//
//    // Libera la memoria delle strutture addrinfo
//    freeaddrinfo(res);
//
//    return 0;

	// setting up log on postgres
	const char * keywords[]={"host","port","dbname","user","password",NULL};
	const char * values[]={"pdb","5432","sistema","server","password",NULL};

	PostgreLogger *pgLogger=new PostgreLogger(1024,keywords,values);
	pgLogger->startLogLoop();



	MyServer server(5,5000,100,100,pgLogger);
	server.start();

	return 0;
}
