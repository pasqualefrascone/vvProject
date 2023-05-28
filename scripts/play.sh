#!/bin/bash
#        std::cout << "Usage: ./server serverName port" << std::endl;
#        std::cout << "Usage: ./client hostname port" << std::endl;
if [ $1 == start ]; then
  ./scripts/build.sh deploy
  ./scripts/run.sh deploy 4
  echo "waiting for postgres..."
  sleep 85
  echo "start playing.."
  docker cp numeri.txt cnode1:/app
  docker cp numeri.txt cnode2:/app
  docker cp numeri.txt cnode3:/app

  echo "docker exec -d cnode0 ./server server0 5000"
  docker exec -d cnode0 bash -c "./server server0 5000"
  sleep 15
  echo "docker exec -d cnode1 bash -c \"./client cnode0 5000 <numeri.txt\""
  docker exec -d cnode1 bash -c "./client cnode0 5000 <numeri.txt"
  echo "docker exec -d cnode2 bash -c \"./client cnode0 5000 <numeri.txt\""
  sleep 5
  docker exec -d cnode2 bash -c "./client cnode0 5000 <numeri.txt"
  echo "docker exec -d cnode3 bash -c \"./client cnode0 5000 <numeri.txt\""
  sleep 5
  docker exec -d cnode3 bash -c "./client cnode0 5000 <numeri.txt"

  #cat numeri.txt | docker exec -i cnode2 ./client cnode0 5000 > /dev/null 2>&1
  #cat numeri.txt | docker exec -i cnode3 ./client cnode0 5000 > /dev/null 2>&1
  echo "done ... you can check on the database..."
  sleep 5
  ./query.sh
  sleep 10
  ./query.sh
  sleep 10
  ./query.sh
fi