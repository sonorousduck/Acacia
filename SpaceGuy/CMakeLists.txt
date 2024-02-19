set(HEADER_FILES 

)

set(SOURCE_FILES
  SpaceGuy.cpp
)

set(SystemHeaders
  systems/aiSystem.hpp
  systems/enemyDetectionSystem.hpp
)

set(SystemSources
  systems/aiSystem.cpp
  systems/enemyDetectionSystem.cpp
)

set(ComponentHeaders
  components/tile.hpp
  components/selectedTile.hpp
  components/playerInformation.hpp
  components/playerShootingInformation.hpp
  components/enemyInformation.hpp
  components/bulletComponent.hpp
)

set(MiscHeaders
  misc/collisionLayers.hpp
  misc/spaceGuyTiledProcessor.hpp
  misc/tileType.hpp
  misc/aiInformationTypes.hpp
)

set(MiscSources
  misc/spaceGuyTiledProcessor.cpp
)


set(Prefabs
  prefabs/mainMusicPrefab.hpp
  prefabs/menuCursorPrefab.hpp
  prefabs/player.hpp
  prefabs/playerBullet.hpp
  prefabs/enemyBullet.hpp
  prefabs/smallEnemyPrefab.hpp
  prefabs/largerEnemyPrefab.hpp
  prefabs/healthBarPrefab.hpp
  prefabs/explosionDeathPrefab.hpp
  prefabs/spawnerPrefab.hpp
  prefabs/turretPrefab.hpp
  prefabs/keyPrefab.hpp
  prefabs/lockedDoorPrefab.hpp
  prefabs/proximityLockedDoorPrefab.hpp
  prefabs/missileRefillPrefab.hpp
  prefabs/potionPrefab.hpp
  prefabs/rapidFirePrefab.hpp
  prefabs/shotgunPrefab.hpp
  prefabs/speedBoostPrefab.hpp
  prefabs/bossPrefab.hpp
  prefabs/playAgainPrefab.hpp
  prefabs/quitInformationPrefab.hpp
  prefabs/gameOverControlsPrefab.hpp
)

set(UIPrefabs
  prefabs/UI/buttonPrefab.hpp
  prefabs/UI/buttonTextPrefab.hpp
  prefabs/UI/playerHealthPrefab.hpp
  prefabs/UI/playerLivesPrefab.hpp
  prefabs/UI/playerMissilePrefab.hpp
  prefabs/UI/playerRadarPrefab.hpp
  prefabs/UI/playerScorePrefab.hpp
  prefabs/UI/playerLivesTextPrefab.hpp
  prefabs/UI/playerMissileTextPrefab.hpp
)

set(SingletonHeaders
  singletons/GameManager.hpp
  singletons/SpaceGuyPythonManager.hpp
)


set(SingletonSources
  singletons/GameManager.cpp
  singletons/SpaceGuyPythonManager.cpp 
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
  scripts/followPlayerCamera.hpp
  scripts/healthBarUpdateScript.hpp
  scripts/enemyMovementShootingScript.hpp
  scripts/playerHealthUpdateScript.hpp
  scripts/playerScoreUpdateScript.hpp
  scripts/spawnerScript.hpp
  scripts/playerKeyUpdateScript.hpp
  scripts/playerLifeUpdateScript.hpp
  scripts/playerMissileUpdateScript.hpp
  scripts/playerOverallLivesText.hpp
  scripts/bossShootingScript.hpp
)

set(PythonScripts

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


add_executable(SpaceGuy ${CODE_FILES} ${src_files_imgui} "../Graphics/thirdparty/glad.c")
set_property(TARGET SpaceGuy PROPERTY CXX_STANDARD 20)
set_target_properties(SpaceGuy PROPERTIES LINKER_LANGUAGE CXX)




if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SpaceGuy PROPERTY CXX_STANDARD 20)
endif()

include_directories("../Ebony/src")
include_directories("../Graphics/src")
target_include_directories(SpaceGuy PRIVATE ../Graphics/include/ ../Graphics/include/glm)


target_link_libraries(SpaceGuy Ebony Graphics glm tileson pybind11::embed)

add_custom_target(copy_assets_space_guy
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(SpaceGuy copy_assets_space_guy)

# TODO: Add tests and install targets if needed.
