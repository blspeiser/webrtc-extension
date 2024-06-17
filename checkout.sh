#!/bin/bash
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH="$PWD/depot_tools:$PATH"
fetch --nohooks webrtc_android
gclient sync
git pull
cd src
sh build/install-build-deps.sh
gclient sync
