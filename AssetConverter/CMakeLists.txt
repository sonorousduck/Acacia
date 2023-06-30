set(CMAKE_CXX_STANDARD 17)

add_executable (baker
"asset_main.cpp")

set_property(TARGET baker PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Ebony>")

target_include_directories(baker PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(baker PUBLIC tiny_obj_loader stb_image rapidjson lz4 assetlib tinyGLTF glm assimp)
