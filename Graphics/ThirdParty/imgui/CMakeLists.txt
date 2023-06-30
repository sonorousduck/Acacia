cmake_minimum_required(VERSION 3.12)
project(Graphics)


set(HEADER_FILES

)

set(SOURCE_FILES

)

set(src_files_imgui
  ThirdParty/imgui/imgui.cpp  
  ThirdParty/imgui/imgui_draw.cpp
  ThirdParty/imgui/imgui_tables.cpp
  ThirdParty/imgui/imgui_widgets.cpp
  ThirdParty/imgui/imgui_impl_glfw.cpp
  ThirdParty/imgui/imgui_impl_opengl3.cpp
  ThirdParty/imgui/imgui_demo.cpp
)

source_group("ImGui" FILES ${src_files_imgui})


add_executable(Graphics ${HEADER_FILES} ${SOURCE_FILES} ${src_files_imgui} src/main.cpp)


target_include_directories(Graphics PRIVATE external/imgui)

target_precompile_headers(Graphics PUBLIC "<unordered_map>" "<vector>" "<iostream>" "<fstream>" "<string>" )
target_link_libraries(Graphics glfw)
set_property(TARGET Graphics PROPERTY CXX_STANDARD 20)
