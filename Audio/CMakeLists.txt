cmake_minimum_required(VERSION 3.12)
project(Audio)


set(HEADER_FILES 
    src/SoundDevice.hpp
    src/SoundBuffer.hpp
    src/SoundSource.hpp
    src/MusicBuffer.hpp
)

set(SOURCE_FILES
    src/main.cpp
    src/SoundDevice.cpp
    src/SoundBuffer.cpp
    src/SoundSource.cpp
    src/MusicBuffer.cpp
)

source_group("Main\\Headers" FILES ${HEADER_FILES})
source_group("Main\\Source Files" FILES ${SOURCE_FILES})



add_executable(Audio ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(Audio OpenAL sndfile)


add_custom_target(copy_assets_sound
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(Audio copy_assets_sound)