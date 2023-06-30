cmake_minimum_required (VERSION 3.8)
set(PROJECT_NAME "Converter")
project(${PROJECT_NAME})

add_executable(converter "asset_main.cpp")

set_property(TARGET converter PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:EbonyGraphicsEngine>")

target_include_directories(converter PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(converter PUBLIC tiny_obj_loader stb_image rapidjson lz4 assetlib assimp glm tinyGLTF)
set_property(TARGET converter PROPERTY CXX_STANDARD 20)