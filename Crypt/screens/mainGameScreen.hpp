#pragma once

#include <screens/screen.hpp>
#include <systems/inputSystem.hpp>
#include <systems/audioSystem.hpp>
#include <systems/fontRenderer.hpp>
#include <systems/physicsSystem.hpp>
#include <systems/spriteRenderer.hpp>
#include <systems/destructionSystem.hpp>
#include <systems/timingSystem.hpp>

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
#include <Log.hpp>
#include <systems/animationRenderer.hpp>
#include <systems/animation2d.hpp>
#include "../systems/playerSystem.hpp"
#include <systems/cppScriptingSystem.hpp>
#include "../systems/shootingSystem.hpp"
#include "../systems/enemyDetectionSystem.hpp"
#include "../misc/cryptTiledProcessor.hpp"


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

		


		void OnScreenDefocus() override;
		void OnScreenFocus() override;

		Ebony::RenderTarget2D mainRenderTarget;

		std::uint64_t screen = Crypt::ScreenEnum::GAME;
		std::uint64_t nextScreen = Crypt::ScreenEnum::GAME;
		Ebony::Color clearColor = Ebony::Colors::CornflowerBlue;
		std::shared_ptr<Camera> camera;

		int windowWidth;
		int windowHeight;

	};
}