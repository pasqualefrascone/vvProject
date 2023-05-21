#!/usr/bin/env bash

#run this script from vvProject folder: ./scripts/run.sh [dev | deploy [numberOfCnode] ]

dev="dev"
deploy="deploy"
clean="clean"

postgresContName="pdb"
cnodeContName="cnode"
netName="mynet"
start_time=$(date +%s.%N)

# Esegui l'operazione
# Sostituisci questa parte con l'azione che desideri eseguire
#echo "Eseguo l'operazione..."
#sleep 3
#
## Calcola il tempo di esecuzione
#end_time=$(date +%s.%N)
#execution_time=$(echo "$end_time - $start_time" | awk '{printf "%.3f", $1}')
#
## Stampa il tempo di esecuzione
echo "execution time: $execution_time seconds"

createNetworkIfNotExist(){
if ! docker network ls | awk -v parametro="$1" '$3==parametro {print $3}' | grep -q .
then
  echo "creating network for postgres.."
  docker network create $1
fi
}

deleteNetworkIfExist(){
  if  docker network ls | awk -v parametro="$1" '$3==parametro {print $3}' | grep -q .
  then
    echo "deleting postgres network.."
    docker network rm $1
  fi

}

stopContainerBeginWith(){

  container_ids=$(docker ps -aq --filter "name=cnode*")

  # Controlla se ci sono container da stoppare
  if [ -n "$container_ids" ]; then
      # Stoppa i container
      docker stop $container_ids
      echo "Container stoppati con successo."
  else
      echo "Nessun container corrispondente al nome trovato."
  fi
}

if [ $# -lt 1 ]; then
  echo "Must specified at least one of the following set of option: [ {dev} | {deploy}, {clean [dev | deploy]} ]."
  exit 1
fi



if [ $1 == dev ]; then
    #create network if don't exists
    createNetworkIfNotExist ${netName}
    echo "starting pdb container..."
    docker run -d --network $netName -e POSTGRES_PASSWORD=password --name $postgresContName pdb
    echo "execution time: $execution_time seconds"

elif [ $1 == deploy ];  then
   #create network if don't exists
    createNetworkIfNotExist ${netName}

  echo "creating and starting containers.... "
      docker run -d --network $netName -e POSTGRES_PASSWORD=password --name $postgresContName pdb
      echo "parm num $#"
      if [ $# -ge 2 ];
      then
        for((i=0;i<second_param;i++)); do
          docker run -d --rm --network $netName --name cnode$i cnode
        done
      else
          docker run -d --rm --network $netName --name cnode0 cnode
      fi


elif [ $1 == "$clean" ]; then
  if [ $2 == dev ]; then
    echo "cleaning dev images..."
    docker rmi $postgresImageName
    docker rmi $cnodeDevImageName
    echo "execution time: $execution_time seconds"
  elif [ $2 == deploy ];  then
    echo "cleaning deployable images..."
    docker rmi $postgresImageName
    docker rmi $cnodeImageName
    echo "execution time: $execution_time seconds"
  fi
fi


#associa container a network
