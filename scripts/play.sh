#!/bin/bash

# play.sh - Script for simulating a server-client environment
#
# This script automates the simulation of a scenario where there is one server node and four client nodes, all connected to their respective PostgreSQL database containers.
# The simulation follows the following steps:
# 1. Executes the 'build.sh' script to create the necessary Docker images and the 'run.sh' script to start the containers.
# 2. Waits for the PostgreSQL container to start.
# 3. Copies the 'numeri.txt' file to the client nodes' containers.
# 4. Starts the game server and game clients using the 'server' and 'client' executables.
# 5. Displays the game results by querying the database inside the 'cnode0' container using the 'query.sh' script.
# 6. Provides an option to clean up the Docker images and containers created for the simulation.
#
# Usage: ./play.sh
#
# Note: Make sure that the 'build.sh', 'run.sh', and 'query.sh' scripts are present in the 'scripts' directory.
#
# Dependencies: Docker, 'server' and 'client' executables, 'numeri.txt' file
#
# Author: [Your Name]

./scripts/build.sh deploy
./scripts/run.sh deploy 20
echo "Waiting for the PostgreSQL container..."
sleep 20
echo "Starting the simulation.."

for i in {1..19}; do
    docker cp numeri.txt cnode$i:/app >/dev/null
done


echo "docker exec -d cnode0 ./server server0 5000"
docker exec -d cnode0 bash -c "./server server0 5000"
sleep 10


for i in {1..19}; do
    echo "docker exec -d cnode$i bash -c \"./client cnode0 5000 <numeri.txt\""
    docker exec -d cnode$i bash -c "./client cnode0 5000 <numeri.txt"
    sleep 2
done



echo "Analyzing what happened in the database container..."
echo "Query: \"select * from agent.logs order by whenn ASC\""
sleep 7
./scripts/query.sh
echo "###########################################################"
sleep 8
./scripts/query.sh
echo "###########################################################"
sleep 9
./scripts/query.sh

# Prompt for cleaning up Docker
printf "Do you want to clean up Docker from the containers and network created for the project? (y/n): "
read answer

# Compare user's response
case "$answer" in
  [Yy]*)
    # Cleaning up containers
    printf "Cleaning up Docker containers...\n"
    ./scripts/run.sh clean all
    printf "Cleaning container completed.\n"
    ;;
  *)
    printf "Cleaning canceled.\n"
    exit 0
    ;;
esac

# Prompt for cleaning up Docker
printf "Do you want to clean up Docker from the images and volume created for the project? (y/n): "
read answer

# Compare user's response
case "$answer" in
  [Yy]*)
    # Cleaning up images
    printf "Cleaning up Docker images...\n"
    ./scripts/build.sh clean deploy

    printf "Cleaning images completed.\n"
    ;;
  *)
    printf "Cleaning images canceled.\n"
    exit 0
    ;;
esac

printf "Do you want to system prune? (y/n): "
read answer
case "$answer" in
  [Yy]*)
    # Cleaning up images
    printf "docker system prune...\n"
    yes y | docker system prune

    printf "Cleaning completed.\n"
    ;;
  *)
    printf "Cleaning canceled.\n"
    ;;
esac


