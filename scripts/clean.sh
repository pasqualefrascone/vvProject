#!/usr/bin/env bash

./scripts/run.sh clean all
./scripts/build.sh clean all
echo "y\n" | docker system prune