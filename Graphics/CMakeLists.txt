cmake_minimum_required(VERSION 3.12)
project(Graphics)


set(HEADER_FILES 
  src/shader.hpp
  src/camera.hpp
  src/mesh.hpp
  src/model.hpp
  src/spriteRenderer.hpp
  src/particleGenerator.hpp
  src/texture.hpp
  src/graphics2d.hpp
  src/renderTarget2d.hpp
  src/window.hpp
  src/input.hpp
  src/colors.hpp
  src/resourceManager.hpp
  src/spritesheet.hpp
  src/graphics.hpp
  src/spritefont.hpp
  src/subtexture2d.hpp

)

set(SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/Graphics/src/main.cpp
  src/shader.cpp
  src/camera.cpp
  src/mesh.cpp
  src/model.cpp   
  src/spriteRenderer.cpp
  src/particleGenerator.cpp
  src/texture.cpp
  src/graphics2d.cpp
  src/renderTarget2d.cpp
  src/window.cpp
  src/input.cpp
  src/colors.cpp
  src/testmain.cpp
  src/resourceManager.cpp
  src/spritesheet.cpp
  src/spritefont.cpp
  src/subtexture2d.cpp

)

set(SHADERS
	shaders/lighting.vert
	shaders/lighting.frag
	shaders/lightcube.vert
	shaders/lightcube.vert
	shaders/lightingTexture.vert
	shaders/lightingTexture.vert

)

#Add source files
# set(GLOB_RECURSE SOURCE_FILES
# 	${CMAKE_SOURCE_DIR}/Graphics/src/*.c
# 	${CMAKE_SOURCE_DIR}/Graphics/src/*.cpp
# )

# message(STATUS SOURCE_FILES)
# #Add header files
# set(GLOB_RECURSE HEADER_FILES
#  ${CMAKE_SOURCE_DIR}/Graphics/src/*.h
#  ${CMAKE_SOURCE_DIR}/Graphics/src/*.hpp
# )

# set (GLOB_RECURSE SHADERS
# 	${CMAKE_SOURCE_DIR}/Graphics/shaders/*.vert
# 	${CMAKE_SOURCE_DIR}/Graphics/shaders/*.frag
# )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


# OpenGL
find_package(OpenGL REQUIRED)


set(src_files_imgui
  ThirdParty/imgui/imgui.cpp  
  ThirdParty/imgui/imgui_draw.cpp
  ThirdParty/imgui/imgui_tables.cpp
  ThirdParty/imgui/imgui_widgets.cpp
  ThirdParty/imgui/imgui_impl_glfw.cpp
  ThirdParty/imgui/imgui_impl_opengl3.cpp
  ThirdParty/imgui/imgui_demo.cpp
  ThirdParty/imgui/imgui_impl_opengl3_loader.h
)

source_group("ThirdParty\\ImGui" FILES ${src_files_imgui})
source_group("Main\\Headers" FILES ${HEADER_FILES})
source_group("Main\\Source Files" FILES ${SOURCE_FILES})
source_group("Main\\Shaders" FILES ${SHADERS})

add_library (Graphics STATIC ${HEADER_FILES} ${SOURCE_FILES} ${SHADERS} ${src_files_imgui} "thirdparty/glad.c")
#add_executable(Graphics ${HEADER_FILES} ${SOURCE_FILES} ${SHADERS} ${src_files_imgui} "thirdparty/glad.c")
#target_precompile_headers(Graphics PUBLIC "<iostream>")


# Define the include DIRs
target_include_directories(Graphics PRIVATE include/ include/glm)



target_include_directories(Graphics PRIVATE ThirdParty/imgui)

target_link_libraries(Graphics glfw stb_image assetlib assimp ctti freetype)
set_property(TARGET Graphics PROPERTY CXX_STANDARD 20)


add_custom_target(copy_assets
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(Graphics copy_assets)
   

# Create virtual folders to make it look nicer in VS
#if(MSVC_IDE)
	# Macro to preserve source files hierarchy in the IDE
#	macro(GroupSources curdir)
#		file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
#
#		foreach(child ${children})
#			if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
#				GroupSources(${curdir}/${child})
#			else()
#				string(REPLACE "/" "\\" groupname ${curdir})
#				string(REPLACE "src" "Sources" groupname ${groupname})
#				source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
#			endif()
#		endforeach()
#	endmacro()
#
#	# Run macro
#	GroupSources(src)
#endif()