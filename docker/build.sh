#!/usr/bin/bash
#
# Build the docker image and push it on GitHub.
# Use the UID 1001 to match Github user ID on runner.
#
set -e

script_folder=$(dirname "$0")

commit=$(git log -1 --format=%h)
git_url=$(git remote get-url origin)
image_name="ghcr.io/spinell/fuse-build-ubuntu-25.10"
tag="1.0.0"

# Build with user ID 1001 (ID use on Github Action)
docker build \
    -f ${script_folder}/ubuntu-25.10.dockerfile \
    --tag ${image_name}:${tag} \
    --tag ${image_name}:latest \
    --label "org.opencontainers.image.source=${git_url}" \
    --label "org.opencontainers.image.description=Image used to build Fuse"  \
    --build-arg USERID=1001 \
    ${script_folder}


docker push ${image_name}:${tag}
