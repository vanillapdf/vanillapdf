#!/bin/bash
VERSION=$1
if [[ -z "$VERSION" ]]; then
  echo "Usage: $0 <version>"
  exit 1
fi

for FILE in ./vcpkg.json ./ports/vanillapdf/vcpkg.json; do
  if [[ -f "$FILE" ]]; then
    echo "Patching $FILE"
    sed -i -E "s/\"version\": *\"[0-9]+\.[0-9]+\.[0-9]+\"/\"version\": \"$VERSION\"/" "$FILE"
  fi
done
