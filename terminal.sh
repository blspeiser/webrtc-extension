#!/bin/bash
docker run -it -v $PWD/libwebrtc:/home/webrtc --entrypoint /bin/bash webrtc_build_env