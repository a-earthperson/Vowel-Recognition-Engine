cmake_minimum_required(VERSION 3.6)
project(VowelRecognition)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c helper.h featureExtraction.h fft.h config.h dataTypes.h kmeans.h filterBank.h dwt.h)
add_executable(VowelRecognition ${SOURCE_FILES})

set(CMAKE_EXE_LINKER_FLAGS "-framework CoreServices -framework CoreFoundation -framework AudioUnit -framework AudioToolbox -framework CoreAudio")
target_link_libraries(VowelRecognition ${CMAKE_SOURCE_DIR}/libportaudio.a)
