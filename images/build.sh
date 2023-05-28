#!/usr/bin/env bash

#docker build -t pdb -f postgres/Dockerfile.dev.dev .
docker build -t client -f images/client/Dockerfile .

#docker build -t server -f images/server/Dockerfile.dev.dev .
