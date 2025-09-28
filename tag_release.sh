#!/usr/bin/env bash
set -euo pipefail

MESHTASTIC_FW_VER="$(cat main/meshtastic_fw_version)"
GIT_BRANCH="$(git branch --show-current)"
if [ "$GIT_BRANCH" != "main" ]; then
  echo "You are trying to release from a non-main branch, checkout 'main' and try again"
  exit 1
fi
LATEST_RELEASE="$(git describe main --tags --abbrev=0 --match='v*')"
GIT_SHA_SHORT="$(git rev-parse --short HEAD)"
NEW_RELEASE_TAG_NAME="v${MESHTASTIC_FW_VER}+${GIT_SHA_SHORT}"
git tag "$NEW_RELEASE_TAG_NAME"