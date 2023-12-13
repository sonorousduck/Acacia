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
		void LoadContent() override
		{
			Ebony::ResourceManager::LoadTexture("Button_Unpressed.tx", "button_unpressed");
			Ebony::ResourceManager::LoadTexture("Button_Pressed.tx", "button_pressed");
			Ebony::ResourceManager::LoadTexture("Button_Unpressed.tx", "button_unpressed");
			Ebony::ResourceManager::LoadTexture("Button_Hovered.tx", "button_hovered");
			Ebony::ResourceManager::LoadTexture("Logo_BrickBreaker", "logo_brickbreaker");
			Ebony::ResourceManager::LoadTexture("Start_Text.tx", "start_text");
			Ebony::ResourceManager::LoadTexture("Options_Text.tx", "options_text");
			Ebony::ResourceManager::LoadTexture("Quit_Text.tx", "quit_text");


		}

		void Init(int windowWidth, int windowHeight) override
		{
			Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

			this->windowHeight = windowHeight;
			this->windowWidth = windowWidth;

			LoadContent();

			Ebony::Graphics2d::SetMainCamera(camera);

			Shader& s = Ebony::ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
			Ebony::ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");

			Ebony::Graphics2d::InitializeTextDrawing(Ebony::ResourceManager::GetShader("text"));


			s.use();
			s.setInt("image", 0);
			s.setMat4("projection", Ebony::Graphics2d::projection);

			
			// Create prefabs

		}

		void AddNewEntities() override
		{
			for (auto&& entity : newEntities)
			{

				allEntities[entity->getId()] = entity;
			}
			newEntities.clear();
		}

		void RemoveOldEntities() override
		{
			for (auto&& entityId : removeEntities)
			{
				allEntities.erase(entityId);
			}

			removeEntities.clear();
		}

		std::uint16_t Update(std::chrono::microseconds elapsedTime) override
		{
			auto firstTime = std::chrono::system_clock::now();

			std::latch graphDone{ 1 };

			auto taskGraph = Ebony::ThreadPool::instance().createTaskGraph(
				[&graphDone]()
				{
					graphDone.count_down();
				});

			// UI will need physics layer, input system, music, sprite


			//auto task1 = Ebony::ThreadPool::instance().createTask(
			//	taskGraph,
			//	[this, elapsedTime]()
			//	{
			//		animationSystem.Update(elapsedTime);
			//	}
			//);

			Ebony::ThreadPool::instance().submitTaskGraph(taskGraph);
			graphDone.wait();


			return nextScreen;

		}

		void Draw(std::chrono::microseconds elapsedTime) override
		{
			Ebony::Graphics2d::SetRenderTarget(mainRenderTarget, clearColor);

			// Draw things!

			Ebony::Graphics2d::UnbindRenderTarget(clearColor);

			Ebony::Graphics2d::DrawRenderTarget(Ebony::ResourceManager::GetShader("screenTexture"), mainRenderTarget);
		}

		void ProcessInput(std::chrono::microseconds elapsedTime)
		{

		}

		void AddEntity(entities::EntityPtr entity)
		{
			std::lock_guard<std::mutex> lock(mutexEntities);

			newEntities.push_back(entity);
		}

		void RemoveEntity(entities::Entity::IdType id)
		{
			std::lock_guard<std::mutex> lock(mutexEntities);

			removeEntities.insert(id);
		}

	public:
		Ebony::RenderTarget2D mainRenderTarget;
		std::uint16_t screen = BrickBreaker::ScreenEnum::MAIN_MENU;
		std::uint16_t nextScreen = BrickBreaker::ScreenEnum::MAIN_MENU;
		Ebony::Color clearColor = Ebony::Colors::White;

	};
}