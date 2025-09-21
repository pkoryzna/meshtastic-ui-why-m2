#!/usr/bin/env bash

set -e

echo "This script requires https://jpa.kapsi.fi/nanopb/download/ version 0.4.9 to be in PATH"
COMPONENT_DIR=$PWD
mkdir -p src/mesh/generated/
# the nanopb tool seems to require that the .options file be in the current directory!
cd protobufs
protoc --experimental_allow_proto3_optional "--nanopb_out=-S.cpp -v:$COMPONENT_DIR/src/mesh/generated/" -I=../protobufs meshtastic/*.proto