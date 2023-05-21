
#        std::cout << "Usage: ./server serverName port" << std::endl;
#        std::cout << "Usage: ./client hostname port" << std::endl;
if [ $1 == start ]; then
  ./scripts/build.sh deploy
  ./scripts/run.sh deploy 4

  docker exec -i cname0 ./server server0 5000 /dev/null 2>&1
  cat numeri.txt | docker exec -i cname1 ./client cname0 5000 > /dev/null 2>&1
  cat numeri.txt | docker exec -i cname2 ./client cname0 5000 > /dev/null 2>&1
  cat numeri.txt | docker exec -i cname3 ./client cname0 5000 > /dev/null 2>&1

