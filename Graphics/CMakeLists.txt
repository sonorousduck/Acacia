cmake_minimum_required(VERSION 3.12)
project(Graphics)


# Add header files
file(GLOB_RECURSE HEADER_FILES
  ${CMAKE_SOURCE_DIR}/src/*.h
  ${CMAKE_SOURCE_DIR}/src/*.hpp
)


# Add source files
file(GLOB_RECURSE SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/src/*.c
  ${CMAKE_SOURCE_DIR}/src/*.cpp
)

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

source_group("ImGui" FILES ${src_files_imgui})

add_executable(Graphics ${HEADER_FILES} ${SOURCE_FILES} ${src_files_imgui} "thirdparty/glad.c" src/main.cpp)

# Define the include DIRs
target_include_directories(Graphics PRIVATE include/)


target_include_directories(Graphics PRIVATE ThirdParty/imgui)

#target_precompile_headers(Graphics PUBLIC "<iostream>")
target_link_libraries(Graphics glfw)
set_property(TARGET Graphics PROPERTY CXX_STANDARD 20)


if(MSVC_IDE)
	# Macro to preserve source files hierarchy in the IDE
	macro(GroupSources curdir)
		file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)

		foreach(child ${children})
			if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				string(REPLACE "src" "Sources" groupname ${groupname})
				source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()

	# Run macro
	GroupSources(src)
endif()
