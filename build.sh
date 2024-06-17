#!/bin/bash
mkdir ./libwebrtc
cp c*.sh ./libwebrtc
docker build -t webrtc_build_env .
