#!/bin/sh

docker build -t registry.gitlab.com/jurzik/vanillapdf/vanillapdf-centos:latest -f centos_dockerfile .
docker push registry.gitlab.com/jurzik/vanillapdf/vanillapdf-centos:latest

# Clean dangling images after the build
docker rmi $(docker images --filter "dangling=true" -q --no-trunc)