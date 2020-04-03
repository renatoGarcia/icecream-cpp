#!/bin/bash

# This script prints to stdout the Dockerfile of a Docker image with a chosen GCC version
# and Boost 1.71.0. These images are prepared to run the IceCream-cpp tests
#
# All images should be found on https://hub.docker.com/r/renatogarcia/icecream-ci


if [[ $# -ne 1 ]]; then
    echo "usage: ./gcc_gen.sh GCC_VERSION"
    exit 1
fi

echo "FROM renatogarcia/icecream-ci:base

WORKDIR /home/icecream_cpp

RUN nix-env -iA nixpkgs.${1}"
