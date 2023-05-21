#!/usr/bin/env bash

#run this script from vvProject folder: ./scripts/build.sh [dev | deploy]
# Usage ./build.sh [ dev | deploy | clean [ dev | deploy | all ] ]

#
dev="dev"
deploy="deploy"
clean="clean"


serverImageDebugName="serverdebug"
clientImageDebugName="clientdebug"


postgresImageName="pdb"
cnodeImageName="cnode"
postgresVolName="data"

start_time=$(date +%s.%N)


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
    echo "build -t $postgresImageName -f images/postgres/Dockerfile ."
    docker build -t $postgresImageName -f images/postgres/Dockerfile .
    echo "docker build -t $serverImageDebugName -f images/cnode/Dockerfile.dev ."
    docker build -t $serverImageDebugName -f images/cnode/Dockerfile.dev .
    echo "docker build -t $clientImageDebugName -f images/cnode/Dockerfile.dev ."
    docker build -t $clientImageDebugName -f images/cnode/Dockerfile.dev .

    createIfVolumeNotExist $postgresVolName
    
elif [ $1 == deploy ];  then
  echo "building deploy images.."
  echo "build -t $postgresImageName -f images/postgres/Dockerfile ."
  docker build -t $postgresImageName -f images/postgres/Dockerfile .
  echo "docker build -t $cnodeImageName -f images/cnode/Dockerfile.dev ."
  docker build -t $cnodeImageName -f images/cnode/Dockerfile .
  createIfVolumeNotExist $postgresVolName


elif [ $1 == "$clean" ]; then
  if [ $2 == dev ]; then
    echo "cleaning dev images..."
    docker rmi $postgresImageName
    docker rmi $serverImageDebugName
    docker rmi $clientImageDebugName

  elif [ $2 == deploy ];  then
    echo "cleaning deployable images..."
    docker rmi $postgresImageName
    docker rmi $cnodeImageName
  elif [ $2 == all ]; then
    echo "cleaning all images and all volume.."
    docker rmi $postgresImageName
    docker rmi $serverImageDebugName
    docker rmi $clientImageDebugName
    docker rmi $cnodeImageName
    deleteVolumeIfExist $postgresVolName
  fi
fi



