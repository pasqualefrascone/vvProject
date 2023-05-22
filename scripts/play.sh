#!/bin/bash
#        std::cout << "Usage: ./server serverName port" << std::endl;
#        std::cout << "Usage: ./client hostname port" << std::endl;
if [ $1 == start ]; then
  ./scripts/build.sh deploy
  ./scripts/run.sh deploy 3
  echo "waiting for postgres..."
  sleep 35
  echo "start playing.."
  docker cp numeri.txt cnode0:/app
  docker cp numeri.txt cnode1:/app
  docker cp numeri.txt cnode2:/app

  docker exec -d cnode0 ./server server0 5000
  sleep 5
  #docker exec -id cnode1 numeri.txt | ./client cnode0 5000
  #cat numeri.txt | docker exec -i cnode2 ./client cnode0 5000 > /dev/null 2>&1
  docker exec  cnode1 bash -c "cat /app/numeri.txt | ./client cnode0 5000"
  sleep 5
  docker exec -d cnode2 bash -c "cat /app/numeri.txt | ./client cnode0 5000"

  #cat numeri.txt | docker exec -i cnode2 ./client cnode0 5000 > /dev/null 2>&1
  #cat numeri.txt | docker exec -i cnode3 ./client cnode0 5000 > /dev/null 2>&1
  echo "done play"
fi