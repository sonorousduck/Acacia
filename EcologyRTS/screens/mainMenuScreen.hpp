
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

#include "screenEnums.hpp"


namespace EcologyRTS
{
	class MainMenuScreen : public Ebony::Screen
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


		std::shared_ptr<Camera> camera;

		Ebony::RenderTarget2D mainRenderTarget;
		std::uint64_t screen = EcologyRTS::ScreenEnum::MAIN_MENU;
		std::uint64_t nextScreen = EcologyRTS::ScreenEnum::MAIN_MENU;
		Ebony::Color clearColor = Ebony::Colors::CornflowerBlue;

		int windowWidth;
		int windowHeight;

	};
}