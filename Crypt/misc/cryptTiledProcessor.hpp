#pragma once

#include <misc/tiledProcessor.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <components/rigidbodyComponent.hpp>
#include <components/collider.hpp>
#include <misc/resourceManager.hpp>

#include "../prefabs/batPrefab.hpp"
#include "../prefabs/suicideBirdPrefab.hpp"



namespace Crypt
{
	class CryptTiledProcessor : Ebony::TiledProcessor
	{
	public:
		CryptTiledProcessor(entities::EntityPtr player) : player(player) {}
		void ParseMap(const char* filepath, std::function<void(entities::EntityPtr)> AddEntity) override;

		void CreateTranslationFunction() override;

		std::function<void(entities::EntityPtr)> AddEntity;
	
	private:
		entities::EntityPtr player;
	};
}
