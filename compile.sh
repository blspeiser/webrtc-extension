#!/bin/bash
cd /home/webrtc/src
gn gen out/Release --args='is_debug=false is_component_build=false rtc_include_tests=false target_os="android" target_cpu="arm"'
ninja -C out/Release
