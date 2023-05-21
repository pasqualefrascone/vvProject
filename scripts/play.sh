#!/bin/bash
#        std::cout << "Usage: ./server serverName port" << std::endl;
#        std::cout << "Usage: ./client hostname port" << std::endl;
if [ $1 == start ]; then
  #./scripts/build.sh deploy
  ./scripts/run.sh deploy 2
  echo "waiting for postgres..."
  sleep 55
  echo "start playing.."
  docker exec -it cnode0 ./server server0 5000 &
  sleep 2
  cat numeri.txt | docker exec -i cnode1 ./client cnode0 5000 &
  #cat numeri.txt | docker exec -i cnode2 ./client cnode0 5000 > /dev/null 2>&1
  #cat numeri.txt | docker exec -i cnode3 ./client cnode0 5000 > /dev/null 2>&1
  echo "done play"
fi