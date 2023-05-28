#!/usr/bin/env bash

# Script for running and managing containers of the vvProject.
# Run this script from the vvProject folder: ./scripts/run.sh [dev | deploy [numberOfCnode]]

dev="dev"
deploy="deploy"
clean="clean"

postgresContName="pdb"
postgresVolName="data"
cnodeContName="cnode"
netName="mynet"

# Execute the operation
# Replace this part with the action you want to perform
# echo "Performing the operation..."
# sleep 3
#
# # Calculate execution time
# end_time=$(date +%s.%N)
# execution_time=$(echo "$end_time - $start_time" | awk '{printf "%.3f", $1}')
#
# # Print execution time
# echo "execution time: $execution_time seconds"

createNetworkIfNotExist(){
    if ! docker network ls | awk -v parametro="$1" '$3==parametro {print $3}' | grep -q .
    then
        echo "Creating network ${netName}..."
        docker network create $1
    fi
}

deleteNetworkIfExist(){
    if docker network ls | awk -v parametro="$1" '$3==parametro {print $3}' | grep -q .
    then
        echo "Deleting network ${netName}.."
        docker network rm $1
    fi
}

stopContainerBeginWith(){
    echo "Stopping $cnodeContName* containers..."

    container_ids=$(docker ps -aq --filter "name=$1*")

    # Check if there are containers to stop
    if [ -n "$container_ids" ]; then
        # Stop the containers
        docker stop $container_ids
        echo "Containers stopped successfully."
    else
        echo "No containers matching the name found."
    fi
}

if [ $# -lt 1 ]; then
    echo "Must specify at least one of the following options: [ {dev} | {deploy}, {clean [dev | deploy]} ]."
    exit 1
fi

if [ $1 == dev ]; then
    # Create network if it doesn't exist
    createNetworkIfNotExist ${netName}
    echo "Starting ${postgresContName} container..."
    docker run --rm -d --network $netName -e POSTGRES_PASSWORD=password --name $postgresContName  -v "$postgresVolName:/var/lib/postgres/data" pdb
    echo "execution time: $execution_time seconds"

elif [ $1 == deploy ]; then
    # Create network if it doesn't exist
    createNetworkIfNotExist ${netName}

    echo "Creating and starting containers...."
    docker run --rm -d --network $netName -e POSTGRES_PASSWORD=password --name $postgresContName  -v "$postgresVolName:/var/lib/postgres/data" pdb
    echo "parm num $#"
    if [ "$#" -ge 2 ]; then
        for ((i=0;i<$2;i++)); do
            echo "Starting $cnodeContName$i ..."
            docker run -d --rm --network $netName --name $cnodeContName$i -h $cnodeContName$i cnode
        done
    else
        docker run -d --rm --network $netName --name $cnodeContName0 -h $cnodeContName0 cnode
    fi

elif [ $1 == "$clean" ]; then
    if [ $# -lt 2 ]; then
        echo "Must specify at least one of the following set of parameters for the clean option: nodes | all."
        exit 1
    fi
    if [ $2 == nodes ]; then
        stopContainerBeginWith $cnodeContName
    elif [ $2 == all ]; then
        stopContainerBeginWith $cnodeContName
        echo "Stopping $postgresContName container"
        docker container stop $postgresContName
        deleteNetworkIfExist $netName
    fi
fi

# Associate containers with network
