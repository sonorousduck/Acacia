#pragma once

#include <screens/screen.hpp>
#include <systems/inputSystem.hpp>
#include <systems/audioSystem.hpp>
#include <systems/fontRenderer.hpp>
#include <systems/physicsSystem.hpp>
#include <systems/spriteRenderer.hpp>

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

#include "screenEnums.hpp"
#include <Log.hpp>


namespace BrickBreaker
{
	class MainScreen : public Ebony::Screen
	{
	public:
		void LoadContent() override;
		void Start() override;
		void Init(int windowWidth, int windowHeight) override;
		void AddNewEntities() override;

		void SetNextScreen(std::uint16_t nextScreen);

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

		void OnScreenDefocus(std::uint64_t nextScreenEnum) override;
		void OnScreenFocus(std::uint64_t lastScreenEnum) override;

		Ebony::RenderTarget2D mainRenderTarget;
		std::uint64_t screen = BrickBreaker::ScreenEnum::MAIN_MENU;
		std::uint64_t nextScreen = BrickBreaker::ScreenEnum::MAIN_MENU;
		Ebony::Color clearColor = Ebony::Colors::CornflowerBlue;
		std::shared_ptr<Camera> camera;

		int windowWidth;
		int windowHeight;

	};
}