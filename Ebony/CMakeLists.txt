cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "Ebony")
project(${PROJECT_NAME})

set(HEADER_FILES
	src/Application.hpp
	src/Core.hpp
	src/EntryPoint.hpp
	src/Log.hpp
	Ebony.hpp
)

set(SOURCE_FILES
	src/Application.cpp
	src/Log.cpp
)


source_group("Main\\Header Files" FILES ${HEADER_FILES})
source_group("Main\\Source Files" FILES ${SOURCE_FILES})



# Add source to this project's executable.
add_library (Ebony ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(Ebony PROPERTIES LINKER_LANGUAGE CXX)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Ebony PROPERTY CXX_STANDARD 20)
endif()

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


#add_subdirectory(ThirdParty/spdlog)
#include_directories(ThirdParty/spdlog/include)
add_subdirectory(spdlog)
target_link_libraries(Ebony spdlog)
#target_include_directories (Ebony PRIVATE ThirdParty/spdlog)

# TODO: Add tests and install targets if needed.
