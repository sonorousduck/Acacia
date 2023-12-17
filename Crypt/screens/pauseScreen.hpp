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
#include <Log.hpp>


namespace Crypt
{
	class PauseScreen : public Ebony::Screen
	{
	public:
		void LoadContent() override;
		void Start() override;
		void Init(int windowWidth, int windowHeight) override;
		void AddNewEntities() override;

		void SetNextScreen(std::uint16_t nextScreen);

		void RemoveOldEntities() override;
		std::uint16_t Update(std::chrono::microseconds elapsedTime) override;
		void Draw(std::chrono::microseconds elapsedTime) override;
		void ProcessInput(std::chrono::microseconds elapsedTime) override;
		void AddEntity(entities::EntityPtr entity);
		void RemoveEntity(entities::Entity::IdType id);

		systems::SpriteRenderer spriteRenderer;
		systems::PhysicsSystem physicsSystem;
		systems::InputSystem inputSystem;
		systems::AudioSystem audioSystem;

		void OnScreenDefocus() override;
		void OnScreenFocus() override;

		Ebony::RenderTarget2D mainRenderTarget;
		std::uint64_t screen = Crypt::ScreenEnum::PAUSE;
		std::uint64_t nextScreen = Crypt::ScreenEnum::PAUSE;
		Ebony::Color clearColor = Ebony::Colors::CornflowerBlue;

		int windowWidth;
		int windowHeight;

	};
}