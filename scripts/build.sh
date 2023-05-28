#!/usr/bin/env bash

# Script for building and managing images of the vvProject.
# Run this script from the vvProject folder: ./scripts/build.sh [dev | deploy]
# Usage: ./build.sh [dev | deploy | clean [dev | deploy | all]]



dev="dev"
deploy="deploy"
clean="clean"

serverImageDebugName="serverdebug"
clientImageDebugName="clientdebug"

postgresImageName="pdb"
cnodeImageName="cnode"
postgresVolName="data"

start_time=$(date +%s.%N)


# Function to create a volume if it doesn't exist
createIfVolumeNotExist(){
    if ! docker volume ls | awk -v parametro="$1" '$2==parametro {print $2}' | grep -q .
    then
        echo "Creating volume for PostgreSQL.."
        docker volume create $1
    fi
}

# Function to delete a volume if it exists
deleteVolumeIfExist(){
    if docker volume ls | awk -v parametro="$1" '$2==parametro {print $2}' | grep -q .
    then
        echo "Deleting volume for PostgreSQL.."
        docker volume rm $1
    fi
}

# Function to build an image if it doesn't exist
buildPdbImageIfNotExist(){
    if docker images $1 | grep -q $1; then
        echo "An image with the same tag $1 already exists. Building will not be performed."
    else
        # Image doesn't exist, execute the command to build it
        docker build -t $1 -f images/postgres/Dockerfile .
    fi
}
promptCleanPdbVolume(){
  printf "Do you want to delete pdb volume? (y/n): "
  read answer

  # Compare user's response
  case "$answer" in
    [Yy]*)
      # Cleaning up containers
      deleteVolumeIfExist $postgresVolName
      ;;
    *)
      exit 0
      ;;
  esac

}


if [ $# -lt 1 ]; then
    echo "Must specify at least one of the following options: [ {dev} | {deploy}, {clean [dev | deploy]} ]."
    exit 1
fi

if [ $1 == dev ]; then
    echo "Building development images..."
    buildPdbImageIfNotExist $postgresImageName
    docker build -t $serverImageDebugName -f images/cnode/Dockerfile.dev .
    docker build -t $clientImageDebugName -f images/cnode/Dockerfile.dev .
    createIfVolumeNotExist $postgresVolName

elif [ $1 == deploy ]; then
    echo "Building deployable images..."
    buildPdbImageIfNotExist $postgresImageName
    docker build -t $cnodeImageName -f images/cnode/Dockerfile .
    createIfVolumeNotExist $postgresVolName

elif [ $1 == all ]; then
    buildPdbImageIfNotExist $postgresImageName
    docker build -t $serverImageDebugName -f images/cnode/Dockerfile.dev .
    docker build -t $clientImageDebugName -f images/cnode/Dockerfile.dev .
    docker build -t $cnodeImageName -f images/cnode/Dockerfile .
    createIfVolumeNotExist $postgresVolName

elif [ $1 == "$clean" ]; then
    if [ $2 == dev ]; then
        echo "Cleaning development images..."
        docker rmi $postgresImageName
        docker rmi $serverImageDebugName
        docker rmi $clientImageDebugName
        promptCleanPdbVolume

    elif [ $2 == deploy ]; then
        echo "Cleaning deployable images..."
        docker rmi $postgresImageName
        docker rmi $cnodeImageName
        promptCleanPdbVolume

    elif [ $2 == all ]; then
        echo "Cleaning all images and volumes..."
        docker rmi $postgresImageName
        docker rmi $serverImageDebugName
        docker rmi $clientImageDebugName
        docker rmi $cnodeImageName
        promptCleanPdbVolume
    fi
fi




