cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "Ebony")
project(${PROJECT_NAME})

set(HEADER_FILES
	src/Application.hpp
	src/Core.hpp
	src/EntryPoint.hpp
	src/Ebony.hpp
	src/animation.hpp
)

set(SOURCE_FILES
	src/Application.cpp
)

set(ComponentHeaders
	src/components/component.hpp
	src/components/animationControllerComponent.hpp
	src/components/particle.hpp
	src/components/controllerComponent.hpp
	src/components/keyboardComponent.hpp
	src/components/mouseInputComponent.hpp
	src/components/audioSource.hpp
	src/components/text.hpp
	src/components/collider.hpp
	src/components/subcolliderClass.hpp
	src/components/rigidbodyComponent.hpp
	src/components/transform.hpp
	src/components/sprite.hpp
	src/components/colliderSprite.hpp
	src/components/soundEffectComponent.hpp
	src/components/musicComponent.hpp
	src/components/aiComponent.hpp
	src/components/activeUICollisionComponent.hpp
	src/components/buttonComponent.hpp
	src/components/cppScriptComponent.hpp
	src/components/destructionComponent.hpp
	src/components/timedComponent.hpp
	src/components/pythonScriptComponent.hpp
	src/components/aiInputComponent.hpp
)

set(SystemHeaders
	src/systems/animation2d.hpp
	src/systems/system.hpp
	src/systems/animationRenderer.hpp
	src/systems/particleRenderer.hpp
	src/systems/particleSystem.hpp
	src/systems/inputSystem.hpp
	src/systems/audioSystem.hpp
	src/systems/fontRenderer.hpp
	src/systems/physicsSystem.hpp
	src/systems/spriteRenderer.hpp
	src/systems/colliderRenderer.hpp
	src/systems/aiSystem.hpp
	src/systems/cppScriptingSystem.hpp
	src/systems/destructionSystem.hpp
	src/systems/timingSystem.hpp
	src/systems/pythonScriptingSystem.hpp
	src/systems/aiInputSystem.hpp
)

set(SystemSources
	src/systems/animation2d.cpp
	src/systems/system.cpp
	src/systems/animationRenderer.cpp
	src/systems/particleRenderer.cpp
	src/systems/particleSystem.cpp
	src/systems/inputSystem.cpp
	src/systems/audioSystem.cpp
	src/systems/fontRenderer.cpp
	src/systems/physicsSystem.cpp
	src/systems/spriteRenderer.cpp
	src/systems/colliderRenderer.cpp
	src/systems/aiSystem.cpp
	src/systems/cppScriptingSystem.cpp
	src/systems/destructionSystem.cpp
	src/systems/timingSystem.cpp
	src/systems/pythonScriptingSystem.cpp
	src/systems/aiInputSystem.cpp

)

set(EntityHeaders
	src/entity.hpp


)

set(EntitySources
	src/entity.cpp

)

set(ScreenHeaders
	src/screens/screen.hpp
)

set(Prefabs
	src/prefabs/templatePrefab.hpp
)


set(MiscHeaders
	src/misc/ThreadPool.hpp
	src/misc/Task.hpp
	src/misc/WorkerThread.hpp
	src/misc/ConcurrentTaskGraph.hpp
	src/misc/ConcurrentQueue.hpp
	src/misc/resourceManager.hpp
	src/misc/Quadtree.hpp
	src/misc/tiledProcessor.hpp
	src/misc/renderLayers.hpp
	src/misc/actionSpaces.hpp
	src/misc/Grid.hpp
)

set(MiscSources
	src/misc/ThreadPool.cpp
	src/misc/Task.cpp
	src/misc/WorkerThread.cpp
	src/misc/ConcurrentTaskGraph.cpp
	src/misc/resourceManager.cpp
	src/misc/Quadtree.cpp
	src/misc/tiledProcessor.cpp
	src/misc/Grid.cpp
)

set (SingletonHeaders
	src/singletons/inputPersistence.hpp
	src/singletons/audioManager.hpp
	src/singletons/time.hpp
	src/singletons/systemManager.hpp
	src/singletons/pythonManager.hpp
)

set (SingletonSources
	src/singletons/inputPersistence.cpp
	src/singletons/audioManager.cpp
	src/singletons/time.cpp
	src/singletons/inputManager.cpp
	src/singletons/systemManager.cpp
	src/singletons/pythonManager.cpp
)

source_group("Main\\Header Files" FILES ${HEADER_FILES})
source_group("Main\\Source Files" FILES ${SOURCE_FILES})
source_group("Components" FILES ${ComponentHeaders})
# source_group("Components\\Source Files" FILES ${ComponentSources})
source_group("Systems\\Header Files" FILES ${SystemHeaders})
source_group("Systems\\Source Files" FILES ${SystemSources})
source_group("Entities\\Header Files" FILES ${EntityHeaders})
source_group("Entities\\Source Files" FILES ${EntitySources})
source_group("Misc\\Header Files" FILES ${MiscHeaders})
source_group("Misc\\Source Files" FILES ${MiscSources})
source_group("Singleton\\Header Files" FILES ${SingletonHeaders})
source_group("Singleton\\Source Files" FILES ${SingletonSources})
source_group("Screens" FILES ${ScreenHeaders})
source_group("Prefabs" FILES ${Prefabs})



source_group("Main\\Event Files" FILES ${HEADER_EVENTS})


set(CODE_FILES
	${HEADER_FILES}
	${SOURCE_FILES}
	${ComponentHeaders}
	${ComponentSources}
	${SystemHeaders}
	${SystemSources}
	${EntityHeaders}
	${EntitySources}
	${MiscHeaders}
	${MiscSources}
	${SingletonHeaders}
	${SingletonSources}
	${ScreenHeaders}
	${Prefabs}
)


# Add source to this project's executable.
add_library (Ebony ${CODE_FILES})
#add_executable (Ebony ${CODE_FILES})

set_target_properties(Ebony PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Ebony PRIVATE ../Graphics/include/ ../Graphics/include/glm)
target_include_directories(Ebony PRIVATE ../Graphics/src)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Ebony PROPERTY CXX_STANDARD 20)
endif()

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
# target_include_directories(Ebony PRIVATE ${Python3_INCLUDE_DIRS})


target_link_libraries(Ebony Graphics rapidjson tileson pybind11::module)

# add_subdirectory(ThirdParty/rapidjson)
# include_directories(ThirdParty/rapidjson/include)

# TODO: Add tests and install targets if needed.
