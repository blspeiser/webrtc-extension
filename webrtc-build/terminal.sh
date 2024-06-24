#!/bin/bash
export PROJECT_ROOT=$(dirname $(readlink -e $PWD))
docker run -it -v $PROJECT_ROOT/libwebrtc:/home/webrtc --entrypoint /bin/bash webrtc_build_env