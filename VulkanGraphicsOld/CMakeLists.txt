cmake_minimum_required(VERSION 3.12)
project(VulkanEngineNotUsed)



#
# Manually specifying all the header files.
#
set(HEADER_FILES
	src/vk_engine.hpp
	src/vk_initializers.hpp
	src/vk_types.hpp
	src/vk_mesh.hpp
	src/vk_textures.hpp
)

#
# Manually specifying all the source files.
#
set(SOURCE_FILES
	src/vk_engine.cpp
	src/vk_initializers.cpp
	src/vk_mesh.cpp
	src/vk_textures.cpp
)

# set(IMGUI_SOURCES
# 	external/imgui/imgui.cpp
# 	external/imgui/imgui_draw.cpp
# 	external/imgui/imgui_widgets.cpp
# )

set(src_files_imgui
  external/imgui/imgui.cpp  
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_vulkan.cpp
  external/imgui/imgui_demo.cpp
)

source_group("ImGui" FILES ${src_files_imgui})


# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#
# This is the main target
#
add_executable(EbonyGraphicsEngine ${HEADER_FILES} ${SOURCE_FILES} ${src_files_imgui} src/main.cpp)
# add_executable(EbonyGraphicsEngine ${HEADER_FILES} ${SOURCE_FILES} src/main.cpp)
#
# We want the C++ 20 standard for our project
#
set_property(TARGET EbonyGraphicsEngine PROPERTY CXX_STANDARD 20)

#
# Enable a lot of warnings for both compilers, forcing the developer to write better code
#
# if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
# 	target_compile_options(EbonyGraphicsEngine PRIVATE /W4 /permissive-)  #/WX will treat warnings as errors
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
# 	target_compile_options(EbonyGraphicsEngine PRIVATE -Wall -Wextra -pedantic -O3) # -Wconversion -Wsign-conversion
# endif()

# target_link_libraries(EbonyGraphicsEngine glfw)
# target_link_libraries(EbonyGraphicsEngine vk-bootstrap)

# target_include_directories(EbonyGraphicsEngine PUBLIC external/tiny_obj_loader)
# target_include_directories(EbonyGraphicsEngine PUBLIC external/stb_image)
# target_include_directories(EbonyGraphicsEngine PRIVATE external/vk_mem_alloc)
target_include_directories(EbonyGraphicsEngine PRIVATE external/imgui)


# find_package(Vulkan REQUIRED)
# target_link_libraries(EbonyGraphicsEngine Vulkan::Vulkan)


add_custom_target(copy_assets
COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)
add_dependencies(EbonyGraphicsEngine copy_assets)

# target_precompile_headers(EbonyGraphicsEngine PUBLIC "src/vk_types.hpp" "<unordered_map>" "<vector>" "<iostream>" "<fstream>" "<string>")
# target_link_libraries(EbonyGraphicsEngine tiny_obj_loader stb_image)



set_property(TARGET EbonyGraphicsEngine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:EbonyGraphicsEngine>")

target_include_directories(EbonyGraphicsEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_precompile_headers(EbonyGraphicsEngine PUBLIC "src/vk_types.hpp" "<unordered_map>" "<vector>" "<iostream>" "<fstream>" "<string>" )
target_link_libraries(EbonyGraphicsEngine vkbootstrap vk_mem_alloc tiny_obj_loader stb_image)
target_link_libraries(EbonyGraphicsEngine Vulkan::Vulkan glfw assetlib)


