# CMakeList.txt : CMake project for Ebony, include source and define
# project specific logic here.
#

# Add source to this project's executable.


set(HEADER_FILES 

)

set(SOURCE_FILES
	SandboxApplication.cpp
)


set(src_files_imgui
  ../Graphics/ThirdParty/imgui/imgui.cpp  
  ../Graphics/ThirdParty/imgui/imgui_draw.cpp
  ../Graphics/ThirdParty/imgui/imgui_tables.cpp
  ../Graphics/ThirdParty/imgui/imgui_widgets.cpp
  ../Graphics/ThirdParty/imgui/imgui_impl_glfw.cpp
  ../Graphics/ThirdParty/imgui/imgui_impl_opengl3.cpp
  ../Graphics/ThirdParty/imgui/imgui_demo.cpp
  ../Graphics/ThirdParty/imgui/imgui_impl_opengl3_loader.h
)

source_group("ThirdParty\\ImGui" FILES ${src_files_imgui})
source_group("Main\\Headers" FILES ${HEADER_FILES})
source_group("Main\\Source Files" FILES ${SOURCE_FILES})


add_executable(Sandbox ${HEADER_FILES} ${SOURCE_FILES} ${src_files_imgui} "../Graphics/thirdparty/glad.c")
set_property(TARGET Sandbox PROPERTY CXX_STANDARD 20)
set_target_properties(Sandbox PROPERTIES LINKER_LANGUAGE CXX)




if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sandbox PROPERTY CXX_STANDARD 20)
endif()

include_directories("${PROJECT_SOURCE_DIR}/Ebony")
include_directories("${PROJECT_SOURCE_DIR}/Graphics/src")
target_include_directories(Sandbox PRIVATE ../Graphics/include/ ../Graphics/include/glm)


target_link_libraries(Sandbox Ebony Graphics glm Audio)

# TODO: Add tests and install targets if needed.
