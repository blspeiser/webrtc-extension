#!/bin/bash
git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
export PATH="$PWD/depot_tools:$PATH"
fetch --nohooks webrtc_android
gclient sync
git pull
cd src
sh build/install-build-deps.sh --android --arm
gclient sync
echo Overriding broken BUILD.gn files with patched versions...
cp -v -f ../patches/build/config/BUILD.gn ./build/config
cp -v -f ../patches/buildtools/third_party/libunwind/BUILD.gn ./buildtools/third_party/libunwind/BUILD.gn
