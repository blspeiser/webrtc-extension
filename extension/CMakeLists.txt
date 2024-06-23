cmake_minimum_required(VERSION 3.22.1)
project("srtp" VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(WEBRTC_SRC ${PROJECT_SOURCE_DIR}/../libwebrtc/src)
set(WEBRTC_BUILD ${WEBRTC_SRC}/out/Release)
set(BORINGSSL_SRC ${PROJECT_SOURCE_DIR}/source/third_party/boringssl/src)
set(BORINGSSL_BUILD ${PROJECT_SOURCE_DIR}/source/third_party/boringssl/src/build)

file(GLOB_RECURSE CUSTOM_STL_OBJS ${WEBRTC_BUILD}/obj/buildtools/third_party/libc++/libc++/*.o)
file(GLOB_RECURSE CUSTOM_ABI_OBJS ${WEBRTC_BUILD}/obj/buildtools/third_party/libc++abi/libc++abi/*.o)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER_TARGET "arm-linux-androideabi32")
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
        ${WEBRTC_BUILD}/obj/libwebrtc.a
        ${BORINGSSL_BUILD}/libcrypto.so
        ${CUSTOM_STL_OBJS}
        ${CUSTOM_ABI_OBJS}
)