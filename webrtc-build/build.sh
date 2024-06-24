#!/bin/bash
mkdir ../libwebrtc
cp checkout.sh ../libwebrtc
cp compile.sh  ../libwebrtc
cp -r patches  ../libwebrtc
docker build -t webrtc_build_env .
