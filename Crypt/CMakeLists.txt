set(HEADER_FILES 

)

set(SOURCE_FILES
	Crypt.cpp
)

set(SystemHeaders
  systems/playerSystem.hpp
  systems/bulletSystem.hpp
  systems/shootingSystem.hpp
  systems/enemyDetectionSystem.hpp
)

set(SystemSources
  systems/playerSystem.cpp
  systems/bulletSystem.cpp
  systems/shootingSystem.cpp
  systems/enemyDetectionSystem.cpp
)

set(ComponentHeaders
  components/playerComponent.hpp
  components/bulletComponent.hpp
  components/aimComponent.hpp
  components/shootingComponent.hpp
  components/enemyDetectionComponent.hpp
)

set(MiscHeaders
  misc/collisionLayers.hpp
)

set(MiscSources
)


set(Prefabs
  prefabs/menuCursorPrefab.hpp
  prefabs/playerPrefab.hpp
  prefabs/groundPrefab.hpp
  prefabs/crosshairPrefab.hpp
  prefabs/bulletPrefab.hpp
  prefabs/batPrefab.hpp
)

set(UIPrefabs
  prefabs/UI/buttonPrefab.hpp
  prefabs/UI/buttonTextPrefab.hpp
)

set(SingletonHeaders
  singletons/GameManager.hpp
  
)


set(SingletonSources
  singletons/GameManager.cpp
  
)

set(ScreenHeaders
  screens/screenEnums.hpp
  screens/controlsScreen.hpp
  screens/highScoreScreen.hpp
  screens/mainGameScreen.hpp
  screens/mainMenuScreen.hpp
  screens/optionsMenuScreen.hpp
  screens/pauseScreen.hpp
)


set(ScreenSources
  screens/controlsScreen.cpp
  screens/highScoreScreen.cpp
  screens/mainGameScreen.cpp
  screens/mainMenuScreen.cpp
  screens/optionsMenuScreen.cpp
  screens/pauseScreen.cpp
)

set(ScriptHeaders
  scripts/crosshairPositionScript.hpp
  scripts/shootingScript.hpp
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
source_group("Prefabs\\UI" FILES ${UIPrefabs})
source_group("Singletons\\Header Files" FILES ${SingletonHeaders})
source_group("Singletons\\Source Files" FILES ${SingletonSources})
source_group("Screens\\Header Files" FILES ${ScreenHeaders})
source_group("Screens\\Source Files" FILES ${ScreenSources})
source_group("Scripts\\Header Files" FILES ${ScriptHeaders})

set (CODE_FILES
  ${HEADER_FILES} 
  ${SOURCE_FILES}
  ${ComponentHeaders}
  ${SystemHeaders}
  ${SystemSources}
  ${MiscHeaders}
  ${MiscSources}
  ${Prefabs}
  ${UIPrefabs}
  ${SingletonHeaders}
  ${SingletonSources}
  ${ScreenHeaders} 
  ${ScreenSources}
  ${ScriptHeaders}
)



add_executable(Crypt ${CODE_FILES} ${src_files_imgui} "../Graphics/thirdparty/glad.c")
set_property(TARGET Crypt PROPERTY CXX_STANDARD 20)
set_target_properties(Crypt PROPERTIES LINKER_LANGUAGE CXX)




if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Crypt PROPERTY CXX_STANDARD 20)
endif()

include_directories("../Ebony/src")
include_directories("../Graphics/src")
target_include_directories(Crypt PRIVATE ../Graphics/include/ ../Graphics/include/glm)


target_link_libraries(Crypt Ebony Graphics glm)

add_custom_target(copy_assets_crypt
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(Crypt copy_assets_crypt)

# TODO: Add tests and install targets if needed.
