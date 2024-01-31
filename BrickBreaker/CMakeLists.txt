set(HEADER_FILES 

)

set(SOURCE_FILES
	BrickBreaker.cpp
)

set(SystemHeaders
  systems/ballSystem.hpp
  systems/lifePointSystem.hpp
  systems/aiSystem.hpp
)

set(SystemSources
  systems/ballSystem.cpp
  systems/lifePointSystem.cpp
  systems/aiSystem.cpp
)

set(ComponentHeaders
  components/ballComponent.hpp
  components/brickComponent.hpp
  components/powerupComponent.hpp
  components/lifeComponent.hpp
  components/scoreComponent.hpp
)

set(MiscHeaders
  misc/collisionLayers.hpp
  misc/screenEnums.hpp
  misc/aiInformationTypes.hpp
)

set(MiscSources
)


set(Prefabs
  prefabs/ballPrefab.hpp
  prefabs/paddlePrefab.hpp
  prefabs/wallPrefab.hpp
  prefabs/brickPrefab.hpp
  prefabs/powerupPrefab.hpp
  prefabs/lifeDisplayPrefab.hpp
  prefabs/lifePrefab.hpp
  prefabs/scorePrefab.hpp
  prefabs/buttonTextPrefab.hpp
  prefabs/logoPrefab.hpp
  prefabs/buttonPrefab.hpp
  prefabs/menuCursorPrefab.hpp
  prefabs/mainMusicPrefab.hpp
)

set(SingletonHeaders
  singletons/GameManager.hpp
  singletons/PythonManager.hpp
)


set(SingletonSources
  singletons/GameManager.cpp
  singletons/PythonManager.cpp
)

set(ScreenHeaders
  screens/screenEnums.hpp
  screens/gameScreen.hpp
  screens/mainScreen.hpp
)


set(ScreenSources
  screens/controlsScreen.cpp
  screens/gameOverScreen.cpp
  screens/gameScreen.cpp
  screens/highScoreScreen.cpp
  screens/mainScreen.cpp
  screens/optionsScreen.cpp
  screens/pauseScreen.cpp
  screens/quitScreen.cpp
)

set(ScriptHeaders
  scripts/DeathScript.hpp

)

set (PythonScriptHeaders
  pythonScripts/main.py
)


set(src_files_imgui
  ../Graphics/ThirdParty/imgui/imgui.cpp  
  ../Graphics/ThirdParty/imgui/imgui_draw.cpp
  ../Graphics/ThirdParty/imgui/imgui_tables.cpp
  ../Graphics/ThirdParty/imgui/imgui_widgets.cpp
  ../Graphics/ThirdParty/imgui/imgui_impl_opengl3.cpp
  ../Graphics/ThirdParty/imgui/imgui_demo.cpp
  ../Graphics/ThirdParty/imgui/imgui_impl_opengl3_loader.h
)

source_group("ThirdParty\\ImGui" FILES ${src_files_imgui})
source_group("Main\\Headers" FILES ${HEADER_FILES})
source_group("Main\\Source Files" FILES ${SOURCE_FILES})
source_group("Systems\\Header Files" FILES ${SystemHeaders})
source_group("Systems\\Source Files" FILES ${SystemSources})
source_group("Components" FILES ${ComponentHeaders})
source_group("Misc\\Header Files" FILES ${MiscHeaders})
source_group("Misc\\Sources Files" FILES ${MiscSources})
source_group("Prefabs" FILES ${Prefabs})
source_group("Singletons\\Header Files" FILES ${SingletonHeaders})
source_group("Singletons\\Source Files" FILES ${SingletonSources})
source_group("Screens\\Header Files" FILES ${ScreenHeaders})
source_group("Screens\\Source Files" FILES ${ScreenSources})
source_group("Scripts\\Header Files" FILES ${ScriptHeaders})
source_group("Python Scripts" FILES ${PythonScriptHeaders})

set (CODE_FILES
  ${HEADER_FILES} 
  ${SOURCE_FILES}
  ${ComponentHeaders}
  ${SystemHeaders}
  ${SystemSources}
  ${MiscHeaders}
  ${MiscSources}
  ${Prefabs}
  ${SingletonHeaders}
  ${SingletonSources}
  ${ScreenHeaders} 
  ${ScreenSources}
  ${ScriptHeaders}
  ${PythonScriptHeaders}
)



add_executable(BrickBreaker ${CODE_FILES} ${src_files_imgui} "../Graphics/thirdparty/glad.c")
set_property(TARGET BrickBreaker PROPERTY CXX_STANDARD 20)
set_target_properties(BrickBreaker PROPERTIES LINKER_LANGUAGE CXX)




if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BrickBreaker PROPERTY CXX_STANDARD 20)
endif()

include_directories("../Ebony/src")
include_directories("../Graphics/src")
target_include_directories(BrickBreaker PRIVATE ../Graphics/include/ ../Graphics/include/glm)


target_link_libraries(BrickBreaker Ebony Graphics glm)

add_custom_target(copy_assets_brick_breaker
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(BrickBreaker copy_assets_brick_breaker)

# TODO: Add tests and install targets if needed.
