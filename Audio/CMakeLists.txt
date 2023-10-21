cmake_minimum_required(VERSION 3.12)
project(Audio)


set(HEADER_FILES 
    src/SoundDevice.hpp
    src/SoundBuffer.hpp
    src/SoundSource.hpp
    src/MusicSource.hpp
    ThirdParty/minimp3/minimp3.h
    ThirdParty/minimp3/minimp3_ex.h
    src/alhelpers.hpp
    src/SourcePool.hpp
    src/EmptySoundDevice.hpp
    src/Sound.hpp
    src/audioManager.hpp
    src/AudioType.hpp
)

set(SOURCE_FILES
    src/main.cpp
    src/SoundDevice.cpp
    src/SoundBuffer.cpp
    src/SoundSource.cpp
    src/MusicSource.cpp
    src/alhelpers.cpp
    src/SourcePool.cpp
    src/EmptySoundDevice.cpp
    src/Sound.cpp
    src/audioManager.cpp

)

source_group("Main\\Headers" FILES ${HEADER_FILES})
source_group("Main\\Source Files" FILES ${SOURCE_FILES})



add_executable(Audio ${HEADER_FILES} ${SOURCE_FILES})
#add_library(Audio ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Audio OpenAL sndfile)




set_property(TARGET Audio PROPERTY CXX_STANDARD 20)


add_custom_target(copy_assets_sound
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(Audio copy_assets_sound)