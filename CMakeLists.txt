cmake_minimum_required (VERSION 3.8)
project ("Ebony")

# find_package(Vulkan REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")


add_compile_definitions(EB_BUILD_DLL EBONY_BUILD_DLL)



# file(GLOB_RECURSE GLSL_SOURCE_FILES
#     "${PROJECT_SOURCE_DIR}/Graphics/shaders/*.frag"  
#     "${PROJECT_SOURCE_DIR}/Graphics/shaders/*.vert"
#     "${PROJECT_SOURCE_DIR}/Graphics/shaders/*.comp"
#     )

#   foreach(GLSL ${GLSL_SOURCE_FILES})
#     message(STATUS "BUILDING SHADER")
#     get_filename_component(FILE_NAME ${GLSL} NAME)
#     set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
#     message(STATUS ${GLSL})
#     add_custom_command(
#       OUTPUT ${SPIRV}
#       COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
#       DEPENDS ${GLSL})
#     list(APPEND SPIRV_BINARY_FILES ${SPIRV})
#   endforeach(GLSL)

#   add_custom_target(
#     Shaders 
#     DEPENDS ${SPIRV_BINARY_FILES}
# 	SOURCES ${GLSL_SOURCE_FILES}
#     )

# Add all sub directories here



# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()



# Include sub-projects.
add_subdirectory ("Ebony")
add_subdirectory ("Sandbox")
add_subdirectory ("Graphics")
add_subdirectory ("AssetLib")
add_subdirectory ("AssetConverter")
add_subdirectory ("ThirdParty")
add_subdirectory ("Audio")

add_custom_target(copy_assets_dll
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(assimp copy_assets_dll)
