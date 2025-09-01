#!/usr/bin/env bash
set -eux
OWNER="pkoryzna"
REPO="meshtastic-firmware-why2025-carrier"

VERSION=$1
if [ -z "$VERSION" ];  then
  exit 1
fi
TAG=v$VERSION

ARTIFACT_FILENAME=firmware-why2025-carrier-$VERSION-update.bin
FILENAME=firmware-why2025-carrier.bin
#mkdir -p build

ARTIFACT_JSON=$(curl -L \
  -H "Accept: application/vnd.github+json" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  https://api.github.com/repos/$OWNER/$REPO/releases/tags/$TAG \
  | tee /dev/stderr | jq ".assets[] | select(.name==\""${ARTIFACT_FILENAME}"\")")

DIGEST=$(echo "$ARTIFACT_JSON" | jq -r .digest)
DOWNLOAD_URL=$(echo "$ARTIFACT_JSON" | jq -r .browser_download_url)

curl -L -o $FILENAME $DOWNLOAD_URL
CHECKSUM=$(echo $DIGEST | cut -d: -f2 | tee $FILENAME.checksum)
ALGO=$(echo $DIGEST | cut -d: -f1 | sed "s/sha//")

shasum --alg $ALGO $FILENAME | grep $CHECKSUM
