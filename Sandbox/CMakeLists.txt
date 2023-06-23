# CMakeList.txt : CMake project for Ebony, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable(Sandbox "SandboxApplication.cpp")
set_target_properties(Sandbox PROPERTIES LINKER_LANGUAGE CXX)


set_property(TARGET Sandbox PROPERTY CXX_STANDARD 20)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sandbox PROPERTY CXX_STANDARD 20)
endif()

include_directories("${PROJECT_SOURCE_DIR}/Ebony")

target_link_libraries(Sandbox Ebony)

# TODO: Add tests and install targets if needed.
