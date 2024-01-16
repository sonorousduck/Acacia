#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <graphics2d.hpp>
#include <SDL.h>
#include <components/controllerComponent.hpp>
#include <components/keyboardComponent.hpp>
#include <components/rigidbodyComponent.hpp>
#include <singletons/time.hpp>
#include <components/mouseInputComponent.hpp>
#include <components/animationControllerComponent.hpp>
#include <components/aiInputComponent.hpp>
#include "../components/playerComponent.hpp"
#include "../screens/screenEnums.hpp"
#include "../components/bulletComponent.hpp"
#include <misc/renderLayers.hpp>
#include "UI/playerScorePrefab.hpp"
#include "singletons/pythonManager.hpp"
#include <misc/actionSpaces.hpp>
#include "../misc/aiInformationTypes.hpp"

namespace Crypt
{
	class Player
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, std::function<void(std::uint64_t)> setNextScreen)
		{
			entities::EntityPtr player = std::make_shared<entities::Entity>();

			glm::vec2 originalVelocity = glm::vec2(100.0f, 500.0f);
			glm::vec2 originalVelocityWithoutY = glm::vec2(originalVelocity.x, 0.0f);

			std::unique_ptr<components::ControllerInput> controllerInputComponent = std::make_unique<components::ControllerInput>(0);
			std::unique_ptr<components::KeyboardInput> keyboardInputComponent = std::make_unique<components::KeyboardInput>();
			std::unique_ptr<components::AiInput> aiInputComponent = std::make_unique<components::AiInput>();




			controllerInputComponent->joystickBindings.insert({ SDL_CONTROLLER_AXIS_LEFTY, "flipGravity" });
			controllerInputComponent->joystickActions.insert({ "flipGravity", [=](float value) {
				if (abs(value) > 0.10)
				{
					auto rigidBody = player->getComponent<components::RigidBody>();
					auto transform = player->getComponent<components::Transform>();
					auto collider = player->getComponent<components::Collider>();
					auto playerComponent = player->getComponent<components::Player>();

					if (playerComponent->gravityCooldown <= 0.0f)
					{
						playerComponent->gravityCooldown = playerComponent->gravityUsageCooldownResetTime;
						playerComponent->gravityDown = !playerComponent->gravityDown;

						transform->rotation = fmod(transform->rotation + 180.0f, 360.0f);
						rigidBody->setVelocity(glm::vec2(rigidBody->getVelocity().x, originalVelocity.y * (playerComponent->gravityDown ? 1 : -1)));
						rigidBody->setAcceleration(glm::vec2(rigidBody->getAcceleration().x, 200.0f * (playerComponent->gravityDown ? 1 : -1)));

						playerComponent->isOnGround = false;
					}
				}
			} });

			aiInputComponent->actions.insert({"flipGravity", [=]()
				{
				auto rigidBody = player->getComponent<components::RigidBody>();
				auto transform = player->getComponent<components::Transform>();
				auto collider = player->getComponent<components::Collider>();
				auto playerComponent = player->getComponent<components::Player>();

				if (playerComponent->gravityCooldown <= 0.0f)
				{
					playerComponent->gravityCooldown = playerComponent->gravityUsageCooldownResetTime;
					playerComponent->gravityDown = !playerComponent->gravityDown;

					transform->rotation = fmod(transform->rotation + 180.0f, 360.0f);
					//std::cout << (playerComponent->gravityDown ? "-1" : "1") << std::endl;
					rigidBody->setVelocity(glm::vec2(rigidBody->getVelocity().x, originalVelocity.y * (playerComponent->gravityDown ? 1 : -1)));
					rigidBody->setAcceleration(glm::vec2(rigidBody->getAcceleration().x, 200.0f * (playerComponent->gravityDown ? 1 : -1)));

					playerComponent->isOnGround = false;
				}
				}
			});
			

			aiInputComponent->actions.insert({ "playerLeft", [=]()
			{
				auto playerComponent = player->getComponent<components::Player>();
				auto rigidBody = player->getComponent<components::RigidBody>();

				if (playerComponent->isSlow)
				{
					playerComponent->isSlow = false;
					rigidBody->setVelocity(rigidBody->getVelocity() + glm::vec2(60.0f, 0.0f));
				}
			}
				});

			aiInputComponent->actions.insert({ "playerRight", [=]()
			{
				auto playerComponent = player->getComponent<components::Player>();
				auto rigidBody = player->getComponent<components::RigidBody>();

				if (playerComponent->isFast)
				{
					playerComponent->isFast = false;
					rigidBody->setVelocity(rigidBody->getVelocity() - glm::vec2(150.0f, 0.0f));
				}
			}
				});

			aiInputComponent->actions.insert({ "speedRelease", [=]()
			{
				auto playerComponent = player->getComponent<components::Player>();
				auto rigidBody = player->getComponent<components::RigidBody>();

				if (playerComponent->isSlow)
				{
					playerComponent->isSlow = false;
					rigidBody->setVelocity(rigidBody->getVelocity() + glm::vec2(60.0f, 0.0f));
				}

				if (playerComponent->isFast)
				{
					playerComponent->isFast = false;
					rigidBody->setVelocity(rigidBody->getVelocity() - glm::vec2(150.0f, 0.0f));
				}

			}
			});



			aiInputComponent->translationFunction = [=]()
				{
					// TODO: Update this so it can figure out its instance ID. But for now, just assume it is the first one
					auto& actionList = Crypt::CryptPythonManager::actions[0];

					if (actionList.size() == 0)
					{
						return;
					}

					auto& action = actionList[0];

					if (action.box[0] == 1.0f)
					{
						player->getComponent<components::AiInput>()->actions["flipGravity"]();
					}

					if (action.box[2] == -1.0f)
					{
						player->getComponent<components::AiInput>()->actions["playerLeft"]();
					}
					else if (action.box[2] == 1.0f)
					{
						player->getComponent<components::AiInput>()->actions["playerRight"]();
					}
					else
					{
						player->getComponent<components::AiInput>()->actions["speedRelease"]();
					}


				};




			controllerInputComponent->joystickBindings.insert({ SDL_CONTROLLER_AXIS_LEFTX, "playerMovement" });

			controllerInputComponent->joystickActions.insert({ "playerMovement", [=](float value) {
				if (abs(value) > 0.10)
				{
					auto rigidBody = player->getComponent<components::RigidBody>();
					auto transform = player->getComponent<components::Transform>();
					auto collider = player->getComponent<components::Collider>();

					if (value < 0)
					{
						rigidBody->setVelocity(originalVelocityWithoutY + glm::vec2(60.0f * value, rigidBody->getVelocity().y));
					}
					else if (value > 0)
					{
						rigidBody->setVelocity(originalVelocityWithoutY + glm::vec2(150.0f * value, rigidBody->getVelocity().y));
					}
				}
			} });

			keyboardInputComponent->onReleaseActions.insert({ "playerLeft", [=]() 
			{ 
				auto playerComponent = player->getComponent<components::Player>();
				auto rigidBody = player->getComponent<components::RigidBody>();

				if (playerComponent->isSlow)
				{
					playerComponent->isSlow = false;
					rigidBody->setVelocity(rigidBody->getVelocity() + glm::vec2(60.0f, 0.0f));
				}
			} 
			});

			keyboardInputComponent->onReleaseActions.insert({ "playerRight", [=]()
			{
				auto playerComponent = player->getComponent<components::Player>();
				auto rigidBody = player->getComponent<components::RigidBody>();

				if (playerComponent->isFast)
				{
					playerComponent->isFast = false;
					rigidBody->setVelocity(rigidBody->getVelocity() - glm::vec2(150.0f, 0.0f));
				}
			}
			});

			

			controllerInputComponent->onPressActions.insert({"pause", [=]()
				{
					Ebony::SystemManager::nextScreenEnum = ScreenEnum::PAUSE;
					//setNextScreen(ScreenEnum::PAUSE);
				} });

			controllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_START, "pause" });


			keyboardInputComponent->onHeldActions.insert({ "print", [=]() { std::cout << "E was called" << std::endl; } });
			keyboardInputComponent->bindings.insert({ SDLK_a, "playerLeft" });
			keyboardInputComponent->bindings.insert({ SDLK_d, "playerRight" });
			keyboardInputComponent->bindings.insert({ SDLK_ESCAPE, "pause" });

			keyboardInputComponent->onPressActions.insert({ "pause", [=]()
				{
					Ebony::SystemManager::nextScreenEnum = ScreenEnum::PAUSE;
				} });


			keyboardInputComponent->onHeldActions.insert({ "playerLeft", [=]()
			{
				auto rigidBody = player->getComponent<components::RigidBody>();
				auto transform = player->getComponent<components::Transform>();
				auto collider = player->getComponent<components::Collider>();
				auto playerComponent = player->getComponent<components::Player>();

				if (!playerComponent->isSlow && !playerComponent->isFast)
				{
					playerComponent->isSlow = true;

					rigidBody->setVelocity(rigidBody->getVelocity() - glm::vec2(60.0f, 0.0f));
				}
			} });

			keyboardInputComponent->onHeldActions.insert({ "playerRight", [=]()
			{
				auto rigidBody = player->getComponent<components::RigidBody>();
				auto transform = player->getComponent<components::Transform>();
				auto collider = player->getComponent<components::Collider>();
				auto playerComponent = player->getComponent<components::Player>();

				if (!playerComponent->isFast && !playerComponent->isSlow)
				{
					playerComponent->isFast = true;

					rigidBody->setVelocity(rigidBody->getVelocity() + glm::vec2(150.0f, 0.0f));
				}
			}
				});

			keyboardInputComponent->bindings.insert({ SDLK_w, "flipGravity" });
			keyboardInputComponent->bindings.insert({ SDLK_SPACE, "flipGravity" });

			keyboardInputComponent->onPressActions.insert({ "flipGravity", [=]()
				{
				auto rigidBody = player->getComponent<components::RigidBody>();
				auto transform = player->getComponent<components::Transform>();
				auto collider = player->getComponent<components::Collider>();
				auto playerComponent = player->getComponent<components::Player>();

				if (playerComponent->gravityCooldown <= 0.0f)
				{
					playerComponent->gravityCooldown = playerComponent->gravityUsageCooldownResetTime;
					playerComponent->gravityDown = !playerComponent->gravityDown;

					transform->rotation = fmod(transform->rotation + 180.0f, 360.0f);
					//std::cout << (playerComponent->gravityDown ? "-1" : "1") << std::endl;
					rigidBody->setVelocity(glm::vec2(rigidBody->getVelocity().x, originalVelocity.y * (playerComponent->gravityDown ? 1 : -1)));
					rigidBody->setAcceleration(glm::vec2(rigidBody->getAcceleration().x, 200.0f * (playerComponent->gravityDown ? 1 : -1)));

					playerComponent->isOnGround = false;
				}
			}
				});

			player->addComponent(std::make_unique<components::Player>());

			//auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("paddle_0"), Ebony::Colors::White);
			
			// Will use a separate system for controlling gravity and such
			auto rigidbody = std::make_unique<components::RigidBody>(originalVelocity, glm::vec2(0.0f, 200.0f));
			auto animationController = std::make_unique<components::AnimationController>(Ebony::RenderLayer::FOREGROUND);


			std::vector<std::chrono::microseconds> timings(4, std::chrono::milliseconds(120));

			std::vector<components::Link> links = { components::Link(1, [=]() {
								return (false);
				
			}) };

			auto spriteSheet = Ebony::ResourceManager::GetTexture("character_run");

			std::vector<Ebony::Animation> animations = { Ebony::Animation(SpriteSheet(spriteSheet, 4, timings), true) };
			
			auto node1 = components::Node(links, animations);

			animationController->animationTree.emplace_back(node1);

			// Debugging only
			// ===================================================================
			//auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::White);
			//player->addComponent(std::move(sprite));


			// ===================================================================
			glm::vec2 scale = glm::vec2(spriteSheet->Width / 4, spriteSheet->Height); // We know there are 4 in the x

			components::Subcollider aabbcollider = components::Subcollider(scale / glm::vec2(2.0f), scale, true, true);

			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					Crypt::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);

					if (layer & Crypt::CollisionLayers::GROUND)
					{
						player->getComponent<components::Player>()->isOnGround = true;
						player->getComponent<components::Transform>()->position.y = player->getComponent<components::Collider>()->aabbCollider.lastCollisionLocation.y;
					}
					else if (layer & Crypt::CollisionLayers::ENEMY_BULLET)
					{
						player->getComponent<components::Player>()->health -= other->getComponent<components::Bullet>()->strength;
					}
				};


			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::PLAYER, Crypt::CollisionLayers::GROUND | Crypt::CollisionLayers::ENEMY_BULLET, false);





			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale, glm::vec3(1.0f, 0.0f, 0.0f));


			Ebony::SystemManager::AddEntity(Crypt::PlayerScore::Create(480, player));

			player->addComponent(std::make_unique<components::AIComponent>(Ebony::AIType::STATE | Ebony::AIType::REWARD, Crypt::AiInformationTypes::PLAYER_INFORMATION));

			player->addComponent(std::move(collider));
			player->addComponent(std::move(animationController));
			player->addComponent(std::move(transform));
			player->addComponent(std::move(keyboardInputComponent));
			player->addComponent(std::move(controllerInputComponent));
			player->addComponent(std::move(rigidbody));
			player->addComponent(std::move(aiInputComponent));

			return player;
		}

	};
}