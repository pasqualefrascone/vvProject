# vvProject



# Readme per il progetto C++, CMake, Docker

Questo readme fornisce una panoramica del progetto, descrive la sua struttura e spiega come eseguire il progetto utilizzando C++, CMake e Docker. Il progetto consiste in un'applicazione C++ che implementa un server TCP con client associati.

## Struttura del progetto

Il progetto è organizzato come segue:

```
+ images
  ---+postgres
      ---------file: Dockerfile
      ---------file: init.sql
      ---------file: postgres.env
  ---+cnode
      ---------file: Dockerfile
+ source
  ---+connprotocol
      ---------file: Client.h
      ---------file: ClientHandler.h
      ---------file: TCPServer.h
  ---+fifo
      ---------files:.....
  ---+loggers
      ---------files:.....
  ---+postgresql
      ---------files:.....
  ---+socket
      ---------files:.....
  ---+utils
      ---------files:.....
+ myClient
  ---file: MyClient.h (estende Client.h)
+ MyServer 
  ---file: MyServer.h (estende TCPServer.h)
  ---file: MyClientHandler (estende clientHandler.h)
+ scripts
  ---file: build.sh (si occupa della costruzione e eliminazione delle immagini e dei volumi)
  ---file: run.sh (si occupa della costruzione e dell'avvio dei container e della rete Docker del progetto)
  ---file: play.sh (uno scenario di gioco con il progetto)
```

## Istruzioni per l'esecuzione

Per eseguire il progetto, seguire i seguenti passaggi:

1. Assicurarsi di avere CMake e Docker installati sul proprio sistema.
2. Clonare il repository del progetto sul proprio computer.
3. Aprire un terminale e posizionarsi nella directory del progetto.

### Costruzione delle immagini Docker

4. Eseguire lo script `build.sh` nella cartella `scripts`. Questo script si occuperà di costruire le immagini Docker necessarie per il progetto, come specificato nei rispettivi Dockerfile.

### Avvio del progetto

5. Eseguire lo script `run.sh` nella cartella `scripts`. Questo script si occuperà di costruire e avviare i container Docker necessari per il progetto, oltre a creare la rete Docker appropriata.

### Scenario di gioco

6. Facoltativamente, è possibile eseguire lo script `play.sh` nella cartella `scripts` per avviare uno scenario di gioco predefinito con il progetto.

Una volta completati questi passaggi, il progetto sarà in esecuzione e pronto per essere utilizzato.

## Conclusioni

Questo readme fornisce una breve panoramica del progetto, descrive la sua struttura e offre istruzioni per l'esecuzione utilizzando C++, CMake e Docker. Assicurarsi di seguire attentamente le istruzioni per garantire il corretto funzionamento del progetto. In caso di problemi o domande, consultare la documentazione o contattare il team di sviluppo del progetto.
