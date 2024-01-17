#pragma once

#include <screens/screen.hpp>
#include <systems/inputSystem.hpp>
#include <systems/audioSystem.hpp>
#include <systems/fontRenderer.hpp>
#include <systems/physicsSystem.hpp>
#include <systems/spriteRenderer.hpp>
#include <systems/destructionSystem.hpp>
#include <systems/timingSystem.hpp>
#include <systems/pythonScriptingSystem.hpp>

#include "camera.hpp"
#include "glm/glm.hpp"
#include "misc/resourceManager.hpp"
#include "spritefont.hpp"
#include "systems/particleSystem.hpp"
#include "misc/ThreadPool.hpp"

#include <filesystem>
#include <latch>
#include <algorithm>

#include "screenEnums.hpp"
#include <systems/animationRenderer.hpp>
#include <systems/animation2d.hpp>
#include "../systems/playerSystem.hpp"
#include "../systems/aiSystem.hpp"
#include <systems/cppScriptingSystem.hpp>
#include <systems/aiInputSystem.hpp>
#include "../systems/shootingSystem.hpp"
#include "../systems/enemyDetectionSystem.hpp"
#include "../misc/cryptTiledProcessor.hpp"
#include "../singletons/CryptPythonManager.hpp"


namespace Crypt
{
	class MainGameScreen : public Ebony::Screen
	{
	public:
		void LoadContent() override;
		void Start() override;
		void Init(int windowWidth, int windowHeight) override;
		void AddNewEntities() override;

		void SetNextScreen(std::uint64_t nextScreen);

		void RemoveOldEntities() override;
		void RemoveAllEntities();

		std::uint64_t Update(std::chrono::microseconds elapsedTime) override;
		void Draw(std::chrono::microseconds elapsedTime) override;
		void ProcessInput(std::chrono::microseconds elapsedTime) override;
		void AddEntity(entities::EntityPtr entity);
		void RemoveEntity(entities::Entity::IdType id);

		systems::SpriteRenderer spriteRenderer;
		systems::PhysicsSystem physicsSystem;
		systems::InputSystem inputSystem;
		systems::AudioSystem audioSystem;
		systems::AnimationRenderer animationRenderer;
		systems::Animation2d animationSystem;
		systems::FontRenderer fontRenderer;
		systems::PlayerSystem playerSystem;
		systems::CppScriptingSystem cppScriptingSystem;
		systems::ShootingSystem shootingSystem;
		systems::DestructionSystem destructionSystem;
		systems::TimingSystem timingSystem;
		systems::EnemyDetectionSystem enemyDetectionSystem;
		systems::PythonScriptingSystem pythonScriptingSystem;
		systems::AIInputSystem aiInputSystem;
		systems::AISystem aiSystem;
		


		void OnScreenDefocus(std::uint64_t nextScreenEnum) override;
		void OnScreenFocus(std::uint64_t lastScreenEnum) override;

		Ebony::RenderTarget2D mainRenderTarget;

		std::uint64_t screen = Crypt::ScreenEnum::GAME;
		std::uint64_t nextScreen = Crypt::ScreenEnum::GAME;
		Ebony::Color clearColor = Ebony::Colors::CornflowerBlue;
		std::shared_ptr<Camera> camera;

		int windowWidth;
		int windowHeight;

	};
}