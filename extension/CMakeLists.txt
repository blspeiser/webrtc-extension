cmake_minimum_required(VERSION 3.22.1)
project("srtp" VERSION 1.0.0 LANGUAGES CXX)


set(WEBRTC_SRC ${PROJECT_SOURCE_DIR}/../libwebrtc/src)
set(WEBRTC_BUILD ${PROJECT_SOURCE_DIR}/../libwebrtc/src/out/Release/obj)
set(BORINGSSL_SRC ${PROJECT_SOURCE_DIR}/../boringssl/src)
set(BORINGSSL_BUILD ${PROJECT_SOURCE_DIR}/../boringssl/src/build)

set(CMAKE_BUILD_TYPE Release)
set(POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)

#====== libsrtp ======
add_library(${CMAKE_PROJECT_NAME} SHARED)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE WEBRTC_LINUX WEBRTC_POSIX)

target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/includes)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC})
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/jsoncpp/source/include/)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WEBRTC_SRC}/third_party/abseil-cpp/)
target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${BORINGSSL_SRC}/include)

target_sources(${CMAKE_PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/source/aes256_frame_encryptor.cpp
        ${PROJECT_SOURCE_DIR}/source/aes256_frame_decryptor.cpp
        ${PROJECT_SOURCE_DIR}/source/io/cambium/webrtc/srtp/Aes256FrameEncryptor.cpp
        ${PROJECT_SOURCE_DIR}/source/io/cambium/webrtc/srtp/Aes256FrameDecryptor.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE
        ${WEBRTC_BUILD}/libwebrtc.a
        ${BORINGSSL_BUILD}/libcrypto.so
        log
)