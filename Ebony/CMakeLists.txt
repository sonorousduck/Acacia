cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME "Ebony")
project(${PROJECT_NAME})

set(HEADER_FILES
	src/Application.hpp
	src/Core.hpp
	src/EntryPoint.hpp
	src/Log.hpp
	src/Ebony.hpp
	src/animation.hpp
)

set(SOURCE_FILES
	src/Application.cpp
	src/Log.cpp
	src/animation.cpp
)

set(HEADER_EVENTS
	src/events/ApplicationEvent.hpp
	src/events/Event.hpp
	src/events/KeyEvent.hpp
	src/events/MouseEvent.hpp
)

set(ComponentHeaders
	src/components/component.hpp
	src/components/animationControllerComponent.hpp
	src/components/particle.hpp
	src/components/inputComponent.hpp
	src/components/audioSource.hpp
)

set(ComponentSources

)

set(SystemHeaders
	src/systems/animation2d.hpp
	src/systems/system.hpp
	src/systems/animationRenderer.hpp
	src/systems/particleRenderer.hpp
	src/systems/staticRenderer.hpp
	src/systems/particleSystem.hpp
	src/systems/inputSystem.hpp
	src/systems/audioSystem.hpp
)

set(SystemSources
	src/systems/animation2d.cpp
	src/systems/system.cpp
	src/systems/animationRenderer.cpp
	src/systems/particleRenderer.cpp
	src/systems/staticRenderer.cpp
	src/systems/particleSystem.cpp
	src/systems/inputSystem.cpp
	src/systems/audioSystem.cpp

)

set(EntityHeaders
	src/entity.hpp

)

set(EntitySources
	src/entity.cpp
)

set(MiscHeaders
	src/misc/ThreadPool.hpp
	src/misc/Task.hpp
	src/misc/WorkerThread.hpp
	src/misc/ConcurrentTaskGraph.hpp
	src/misc/ConcurrentQueue.hpp
)

set(MiscSources
	src/misc/ThreadPool.cpp
	src/misc/Task.cpp
	src/misc/WorkerThread.cpp
	src/misc/ConcurrentTaskGraph.cpp
)


source_group("Main\\Header Files" FILES ${HEADER_FILES})
source_group("Main\\Source Files" FILES ${SOURCE_FILES})
source_group("Components\\Header Files" FILES ${ComponentHeaders})
source_group("Components\\Source Files" FILES ${ComponentSources})
source_group("Systems\\Header Files" FILES ${SystemHeaders})
source_group("Systems\\Source Files" FILES ${SystemSources})
source_group("Entities\\Header Files" FILES ${EntityHeaders})
source_group("Entities\\Source Files" FILES ${EntitySources})
source_group("Misc\\Header Files" FILES ${MiscHeaders})
source_group("Misc\\Source Files" FILES ${MiscSources})


source_group("Main\\Event Files" FILES ${HEADER_EVENTS})


set(CODE_FILES
	${HEADER_FILES}
	${SOURCE_FILES}
	${ComponentHeaders}
	${ComponentSources}
	${SystemHeaders}
	${SystemSources}
	${HEADER_EVENTS}
	${EntityHeaders}
	${EntitySources}
	${MiscHeaders}
	${MiscSources}
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

target_link_libraries(Ebony spdlog Graphics)


# add_subdirectory(ThirdParty/rapidjson)
# include_directories(ThirdParty/rapidjson/include)

# TODO: Add tests and install targets if needed.
