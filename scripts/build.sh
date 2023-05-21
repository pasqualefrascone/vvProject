#!/usr/bin/env bash

#run this script from vvProject folder: ./scripts/build.sh [dev | deploy]

dev="dev"
deploy="deploy"
clean="clean"

postgresImageName="pdb"
cnodeDevImageName="cnodedev"
cnodeImageName="cnode"
pdbVolumeName="data"

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

createIfVolumeNotExist(){
if ! docker volume ls | awk -v parametro="$1" '$2==parametro {print $2}' | grep -q .
then
  echo "creating volume for postgres.."
  docker volume create $1
fi
}

deleteVolumeIfExist(){
  if  docker volume ls | awk -v parametro="$1" '$2==parametro {print $2}' | grep -q .
  then
    echo "deleting volume for postgres.."
    docker volume rm $1
  fi

}


echo "execution time: $execution_time seconds"

if [ $# -lt 1 ]; then
  echo "Must specified at least one of the following set of option: [ {dev} | {deploy}, {clean [dev | deploy]} ]."
  exit 1
fi




if [ $1 == dev ]; then
    echo "building dev images..."
    docker build -t $postgresImageName -f images/postgres/Dockerfile .
    docker build -t $cnodeDevImageName -f images/cnode/Dockerfile.dev .
    echo "execution time: $execution_time seconds"
    createIfVolumeNotExist $pdbVolumeName
    
elif [ $1 == deploy ];  then
  echo "building deploy images.."
  echo $(pwd)
  docker build -t $postgresImageName -f images/postgres/Dockerfile .
  docker build -t $cnodeImageName -f images/cnode/Dockerfile .
  createIfVolumeNotExist $pdbVolumeName


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
  elif [ $2 == all ]; then
    echo "cleaning all images and all volume.."
    docker rmi $postgresImageName
    docker rmi $cnodeDevImageName
    docker rmi $cnodeImageName
    deleteVolumeIfExist $pdbVolumeName
  fi
fi



#associa container a network
