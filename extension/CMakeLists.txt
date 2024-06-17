cmake_minimum_required(VERSION 3.22.1)
project("srtp" VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(WEBRTC_SRC ${PROJECT_SOURCE_DIR}/../libwebrtc/src)
set(WEBRTC_BUILD ${WEBRTC_SRC}/out/Release)

file(GLOB_RECURSE CUSTOM_STL_OBJS ${WEBRTC_BUILD}/obj/buildtools/third_party/libc++/libc++/*.o)
file(GLOB_RECURSE CUSTOM_ABI_OBJS ${WEBRTC_BUILD}/obj/buildtools/third_party/libc++abi/libc++abi/*.o)
file(GLOB_RECURSE CLANG_LIBRARIES /home/sbaruch/android-ndk-r26d/toolchains/llvm/prebuilt/linux-x86_64/lib/clang/17/lib/linux/arm/*.a)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER_TARGET "arm-linux-androideabi32")
set(CMAKE_CXX_COMPILER ${WEBRTC_SRC}/third_party/llvm-build/Release+Asserts/bin/clang)

add_library(${CMAKE_PROJECT_NAME} STATIC)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE WEBRTC_LINUX WEBRTC_POSIX)

target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/includes)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC})
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/perfetto/buildtools/libcxx_config)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/jsoncpp/source/include/)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/abseil-cpp/)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/libc++/src/include)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/buildtools/third_party/libc++)

target_sources(${CMAKE_PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/source/custom_frame_encryptor.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
        ${WEBRTC_BUILD}/obj/libwebrtc.a
        ${CUSTOM_STL_OBJS}
        ${CUSTOM_ABI_OBJS}
)