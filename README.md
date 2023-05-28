# vvProject

## Overview
This readme provides an overview of the project, describes its structure, and explains how to run the project using C++, CMake, and Docker. The project consists of a C++ application that implements a TCP server with associated clients. The client sends two integers, and the server responds with the difference between the received integer pairs. Both types of nodes share most of the code, which can be found in the "source" folder. In the "source" folder, in the "protocol" subfolder, you can find the abstract classes "Client," "TCPServer," and "ClientHandler," which are extended respectively in the "myClient" and "myServer" subfolders.

## Execution Instructions
To run the project, follow these steps:

1. Ensure that CMake and Docker are installed on your system.
2. Clone the project repository to your computer.
3. Open a terminal and navigate to the project directory.

### Building Docker Images
4. Run the `build.sh` script in the `scripts` folder. This script will build the necessary Docker images for the project, as specified in their respective Dockerfiles.
   Please refer to the usage in the initial comment of the script for more information.

### Running the Project
5. Run the `run.sh` script in the `scripts` folder. This script will build and run the required Docker containers for the project, as well as create the appropriate Docker network.
   Please refer to the usage in the initial comment of the script for more information.

### Scenario Simulation

Optionally, you can run the `play.sh` script in the scripts folder to simulate a predefined scenario where there is a server and multiple clients communicating with the server.

Please note that the play.sh script is designed to simulate the interaction between the server and clients in a controlled environment for testing and demonstration purposes.
Once these steps are completed, the project will be up and running and ready to be used.

## Project Structure
The project is organized as follows:

```
+ images
|---+postgres
|------Dockerfile
|------init.sql
|------postgres.env
|---+cnode
|------Dockerfile
+ source
|---+connprotocol
|------Client.h
|------ClientHandler.h
|------TCPServer.h
|---+fifo
|------...
|---+loggers
|------.....
|---+postgresql
|------.....
|---+socket
|------.....
|---+utils
|------.....
+ myClient
|--- MyClient.h (extends Client.h)
+ myServer 
|--- MyServer.h (extends TCPServer.h)
|--- MyClientHandler (extends ClientHandler.h)
+ scripts
|--- build.sh (handles the building and removal of images and volumes)
|--- run.sh (handles the building and running of containers and the Docker network for the project)
|--- play.sh (a game scenario with the project)
```

## Setting Up CLion for Development
To configure CLion for development, follow these additional steps:

1. Ensure that the Docker images are built by executing the command `./scripts/build.sh dev`.
2. Open CLion and go to "Settings" -> "Build, Execution, Deployment" -> "Toolchains" and add two Docker toolchains, one for the client and one for the server.
3. For both the server and client, select the respective Docker image ("clientdebug" or "serverdebug") in the toolchain settings. In the "Container settings" input text, enter "-h clientDebug --network mynet" for the client and "-h serverDebug --network mynet" for the server.
4. Select Docker GDB as the debugger.
5. Create Run/Debug configurations by clicking on the drop-down next to the green hammer icon. Select "Edit Configurations" and add two "CMake Application" configurations.
6. In the "Target" and "Executable" fields, select "client" or "server" respectively. In the "Program arguments" field

, enter "serverDebug 5000" for both the server and client.
7. Optionally, you can install the Docker plugin for CLion, but it is not necessary.

Make sure to follow the instructions carefully to ensure the proper functioning of the project. If you encounter any issues or have questions, refer to the documentation or contact the project's development team.