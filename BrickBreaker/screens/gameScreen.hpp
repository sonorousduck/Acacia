#pragma once

#include <screens/screen.hpp>
#include <systems/particleSystem.hpp>
#include <systems/particleRenderer.hpp>
#include <systems/animationRenderer.hpp>
#include <systems/animation2d.hpp>
#include <systems/inputSystem.hpp>
#include <systems/audioSystem.hpp>
#include <systems/fontRenderer.hpp>
#include <systems/physicsSystem.hpp>
#include <systems/spriteRenderer.hpp>
#include "../systems/ballSystem.hpp"
#include "../systems/lifePointSystem.hpp"

#include "camera.hpp"
#include "glm/glm.hpp"
#include "misc/resourceManager.hpp"
#include "spritefont.hpp"
#include "systems/particleSystem.hpp"
#include "misc/ThreadPool.hpp"

#include <filesystem>
#include <latch>
#include <algorithm>
#include <components/text.hpp>
#include <components/mouseInputComponent.hpp>
#include <singletons/time.hpp>

#include "../components/ballComponent.hpp"
#include "../systems/ballSystem.hpp"
#include "../misc/collisionLayers.hpp"
#include "../components/brickComponent.hpp"

#include "../prefabs/ballPrefab.hpp"
#include "../prefabs/paddlePrefab.hpp"
#include "../prefabs/wallPrefab.hpp"
#include "../prefabs/brickPrefab.hpp"
#include "../prefabs/lifePrefab.hpp"
#include "../prefabs/scorePrefab.hpp"
#include "../systems/lifePointSystem.hpp"
#include "screenEnums.hpp"
#include <systems/cppScriptingSystem.hpp>


namespace BrickBreaker
{
	class GameScreen : public Ebony::Screen
	{
	public:
		void LoadContent() override;
		void Init(int windowWidth, int windowHeight) override;
		void Start() override;
		void AddNewEntities() override;
		void RemoveOldEntities() override;
		std::uint64_t Update(std::chrono::microseconds elapsedTime) override;
		void Draw(std::chrono::microseconds elapsedTime) override;
		void ProcessInput(std::chrono::microseconds elapsedTime) override;
		void AddEntity(entities::EntityPtr entity);
		void RemoveEntity(entities::Entity::IdType id);

		void Reset();


		void OnScreenDefocus(std::uint64_t nextScreenEnum) override;
		void OnScreenFocus(std::uint64_t lastScreenEnum) override;

		std::string fps = "";
		//Ebony::KeyInput keyInput;
		//Ebony::MouseInput mouseInput;
		//Ebony::ControllerInput controllerInput;
		Ebony::Color clearColor;
		//std::shared_ptr<Ebony::SpriteFont> spriteFont;
		systems::ParticleSystem particleSystem;
		systems::ParticleRenderer particleRenderer;
		systems::AnimationRenderer animationRenderer;
		systems::Animation2d animationSystem;
		systems::InputSystem inputSystem;
		systems::AudioSystem audioSystem;
		systems::FontRenderer fontRenderer;
		systems::PhysicsSystem physicsSystem;
		systems::SpriteRenderer spriteRenderer;
		systems::BallSystem ballSystem;
		systems::LifePointSystem lifePointSystem;
		systems::CppScriptingSystem cppScriptingSystem;

		Ebony::RenderTarget2D mainRenderTarget;


		std::chrono::microseconds averageParticleRenderingTime = std::chrono::microseconds::zero();
		std::chrono::microseconds averageParticleSystemTime = std::chrono::microseconds::zero();
		std::chrono::microseconds averageAnimationSystemTime = std::chrono::microseconds::zero();
		std::chrono::microseconds averageUpdateTime = std::chrono::microseconds::zero();
		std::shared_ptr<Camera> camera;

		entities::EntityPtr fpsEntity;
		float fpsUpdateDeltaTime = 0.0f;

		std::uint64_t screen = BrickBreaker::ScreenEnum::GAME;
		std::uint64_t nextScreen = BrickBreaker::ScreenEnum::GAME;

		int windowWidth;
		int windowHeight;

	private:
		void RemoveAllEntities();

	};
}