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
  systems/aiSystem.hpp
)

set(SystemSources
  systems/playerSystem.cpp
  systems/bulletSystem.cpp
  systems/shootingSystem.cpp
  systems/enemyDetectionSystem.cpp
  systems/aiSystem.cpp
)

set(ComponentHeaders
  components/playerComponent.hpp
  components/bulletComponent.hpp
  components/aimComponent.hpp
  components/shootingComponent.hpp
  components/enemyDetectionComponent.hpp
  components/enemyComponent.hpp
  components/selectedSpellComponent.hpp
)

set(MiscHeaders
  misc/collisionLayers.hpp
  misc/cryptTiledProcessor.hpp
  misc/aiInformationTypes.hpp
)

set(MiscSources
  misc/cryptTiledProcessor.cpp
)


set(Prefabs
  prefabs/menuCursorPrefab.hpp
  prefabs/playerPrefab.hpp
  prefabs/groundPrefab.hpp
  prefabs/crosshairPrefab.hpp
  prefabs/bulletPrefab.hpp
  prefabs/batPrefab.hpp
  prefabs/healthBarPrefab.hpp
  prefabs/suicideBirdPrefab.hpp
  prefabs/explosionDeathPrefab.hpp
  prefabs/mainMusicPrefab.hpp
  prefabs/gameOverControlsPrefab.hpp
  prefabs/gameCompletePrefab.hpp
)

set(UIPrefabs
  prefabs/UI/buttonPrefab.hpp
  prefabs/UI/buttonTextPrefab.hpp
  prefabs/UI/playerHealthPrefab.hpp
  prefabs/UI/playerSpellSelection.hpp
  prefabs/UI/spellPrefab.hpp
  prefabs/UI/heartPrefab.hpp
  prefabs/UI/cryptPrefab.hpp
  prefabs/UI/gameOverPrefab.hpp
  prefabs/UI/victoryPrefab.hpp
  prefabs/UI/cryptTitlePrefab.hpp
  prefabs/UI/playAgainPrefab.hpp
  prefabs/UI/quitInformationPrefab.hpp
  prefabs/UI/playerScorePrefab.hpp
)

set(SingletonHeaders
  singletons/GameManager.hpp
  singletons/CryptPythonManager.hpp
)


set(SingletonSources
  singletons/GameManager.cpp
  singletons/CryptPythonManager.cpp 
)

set(ScreenHeaders
  screens/screenEnums.hpp
  screens/controlsScreen.hpp
  screens/highScoreScreen.hpp
  screens/mainGameScreen.hpp
  screens/mainMenuScreen.hpp
  screens/optionsMenuScreen.hpp
  screens/pauseScreen.hpp
  screens/gameOverScreen.hpp
)


set(ScreenSources
  screens/controlsScreen.cpp
  screens/highScoreScreen.cpp
  screens/mainGameScreen.cpp
  screens/mainMenuScreen.cpp
  screens/optionsMenuScreen.cpp
  screens/pauseScreen.cpp
  screens/gameOverScreen.cpp
)

set(ScriptHeaders
  scripts/crosshairPositionScript.hpp
  scripts/shootingScript.hpp
  scripts/suicideBatScript.hpp
  scripts/shootingBatScript.hpp
  scripts/healthBarUpdateScript.hpp
  scripts/playerHealthUpdateScript.hpp
  scripts/playerScoreUpdateScript.hpp
)

set(PythonScripts
  pythonScripts/testPythonScript.py


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
source_group("Scripts" FILES ${ScriptHeaders})
source_group("Scripts\\Python" FILES ${PythonScripts})

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
  ${PythonScripts}
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


target_link_libraries(Crypt Ebony Graphics glm tileson pybind11::embed)

add_custom_target(copy_assets_crypt
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(Crypt copy_assets_crypt)

# TODO: Add tests and install targets if needed.
