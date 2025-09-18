#!/usr/bin/env bash
set -eux
OWNER="pkoryzna"
REPO="meshtastic-firmware-why2025-carrier"

VERSION=$1
if [ -z "$VERSION" ];  then
  exit 1
fi
TAG=v$VERSION

RELEASE_JSON=$(curl -L \
                 -H "Accept: application/vnd.github+json" \
                 -H "X-GitHub-Api-Version: 2022-11-28" \
                 https://api.github.com/repos/$OWNER/$REPO/releases/tags/$TAG \
                 | tee /dev/stderr)

for ARTIFACT_FILENAME in boot_app0.bin bootloader.bin firmware.bin partitions.bin ; do
  FILENAME=c6-$ARTIFACT_FILENAME

  ARTIFACT_JSON=$(echo $RELEASE_JSON | jq ".assets[] | select(.name==\""${ARTIFACT_FILENAME}"\")")

  DIGEST=$(echo "$ARTIFACT_JSON" | jq -r .digest)
  DOWNLOAD_URL=$(echo "$ARTIFACT_JSON" | jq -r .browser_download_url)

  if [ ! -f $FILENAME ]; then
    curl -L -o $FILENAME $DOWNLOAD_URL
  fi

  CHECKSUM=$(echo $DIGEST | cut -d: -f2 | tee $FILENAME.checksum)
  ALGO=$(echo $DIGEST | cut -d: -f1 | sed "s/sha//")

  if ! (shasum --alg "$ALGO" "$FILENAME" | grep "$CHECKSUM"); then
    echo "$FILENAME did not match expected checksum $CHECKSUM!"
    exit 1
  fi

  md5sum $FILENAME | cut -d' ' -f1 > $FILENAME.md5
done