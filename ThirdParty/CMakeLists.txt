# find_package(Vulkan REQUIRED)
project(ThirdParty)

# add_library(vkbootstrap STATIC)


add_library(stb_image INTERFACE)
add_library(rapidjson INTERFACE)
add_library(tiny_obj_loader STATIC)
# add_library(vk_mem_alloc INTERFACE)
add_library(glm INTERFACE)
add_library(sdl2 INTERFACE)
# add_library(freetype INTERFACE)


add_library(lz4 STATIC)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
add_subdirectory(spdlog)



find_package(SDL2 REQUIRED)
target_include_directories(sdl2 INTERFACE ${SDL2_INCLUDE_DIRS})
target_link_libraries(sdl2 INTERFACE ${SDL2_LIBRARIES})

if (WIN32)
    file(COPY ${SDL2_DIR}/../lib/x64/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
endif()

if (WIN64)
    file(COPY ${SDL2_DIR}/../lib/x64/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
endif()

# target_sources(vkbootstrap PRIVATE 
#     "${CMAKE_CURRENT_SOURCE_DIR}/vkbootstrap/VkBootstrap.h"
#     "${CMAKE_CURRENT_SOURCE_DIR}/vkbootstrap/VkBootstrap.cpp"
# )


target_sources(lz4 PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/lz4/lib/lz4.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/lz4/lib/lz4.c"
)

target_include_directories(lz4 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lz4/lib" )
# target_include_directories(vkbootstrap PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vkbootstrap" )
# target_link_libraries(vkbootstrap PUBLIC Vulkan::Vulkan)

#both vma and glm and header only libs so we only need the include path
# target_include_directories(vk_mem_alloc INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/vk_mem_alloc" )
target_include_directories(glm INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/glm")

# add_subdirectory(box2d)

add_subdirectory(gltf)
add_subdirectory(openal-soft)

add_subdirectory(ctti)
include_directories(ctti/include)

# target_sources(vk_mem_alloc PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vk_mem_alloc/vk_mem_alloc.h")

option(RAPIDJSON_BUILD_DOC "Build rapidjson documentation." OFF)
option(RAPIDJSON_BUILD_EXAMPLES "Build rapidjson examples." OFF)
option(RAPIDJSON_BUILD_TESTS "Build rapidjson perftests and unittests." OFF)
option(RAPIDJSON_BUILD_THIRDPARTY_GTEST
    "Use gtest installation in `thirdparty/gtest` by default if available" OFF)
option(RAPIDJSON_BUILD_CXX11 "Build rapidjson with C++11" OFF)
option(RAPIDJSON_BUILD_CXX17 "Build rapidjson with C++17" ON)
option(RAPIDJSON_BUILD_ASAN "Build rapidjson with address sanitizer (gcc/clang)" OFF)
option(RAPIDJSON_BUILD_UBSAN "Build rapidjson with undefined behavior sanitizer (gcc/clang)" OFF)
option(RAPIDJSON_ENABLE_INSTRUMENTATION_OPT "Build rapidjson with -march or -mcpu options" OFF)

target_include_directories(rapidjson INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include")

target_sources(tiny_obj_loader PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/tiny_obj_loader/tiny_obj_loader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/tiny_obj_loader/tiny_obj_loader.cc"
)


target_include_directories(tiny_obj_loader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/tiny_obj_loader" )

# target_link_libraries(imgui PUBLIC Vulkan::Vulkan glfw)

target_include_directories(stb_image INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb_image" )

add_library(assimp INTERFACE)

set(assimp_dir "${CMAKE_CURRENT_SOURCE_DIR}/assimp_build")
set(assimp_dll "${CMAKE_CURRENT_SOURCE_DIR}/assimp_build/Release/assimp-vc143-mt.dll")
set(assimp_lib "${CMAKE_CURRENT_SOURCE_DIR}/assimp_build/Release/assimp-vc143-mt.lib")

target_include_directories(assimp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/assimp/include")
target_link_libraries(assimp INTERFACE ${assimp_lib} )

# add_library(sndfile INTERFACE)
# target_include_directories(sndfile INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libsndfile/include")

# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# find_package(SndFile REQUIRED)

add_subdirectory(libsndfile)
add_subdirectory(freetype)
set(freetype_lib "${CMAKE_CURRENT_SOURCE_DIR}/freetype/build/Release/freetype.lib")


#target_include_directories(freetype INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/freetype/include")
target_link_libraries(freetype INTERFACE ${freetype_lib})