cmake_minimum_required(VERSION 3.22.1)
project("srtp" VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(WEBRTC_SRC ${PROJECT_SOURCE_DIR}/../libwebrtc/src)
set(WEBRTC_BUILD ${PROJECT_SOURCE_DIR}/../prebuilt/ubuntu-20.04/ndk-26.3.11579264/arm64-v8a/android-32)
set(BORINGSSL_SRC ${PROJECT_SOURCE_DIR}/../boringssl/src)
set(BORINGSSL_BUILD ${PROJECT_SOURCE_DIR}/../boringssl/src/build)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER ${WEBRTC_SRC}/third_party/llvm-build/Release+Asserts/bin/clang)

#====== libsrtp ======
add_library(${CMAKE_PROJECT_NAME} STATIC)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE WEBRTC_LINUX WEBRTC_POSIX)

target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/includes)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC})
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/perfetto/buildtools/libcxx_config)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/jsoncpp/source/include/)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/abseil-cpp/)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/libc++/src/include)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/buildtools/third_party/libc++)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${BORINGSSL_SRC}/include)

target_sources(${CMAKE_PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/source/aes256_frame_encryptor.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
        ${WEBRTC_BUILD}/libjingle_peerconnection_so.so
        ${BORINGSSL_BUILD}/libcrypto.so
)