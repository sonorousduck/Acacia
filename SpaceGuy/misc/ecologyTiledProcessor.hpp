#pragma once

#include <misc/tiledProcessor.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <components/rigidbodyComponent.hpp>
#include <components/collider.hpp>
#include <misc/resourceManager.hpp>




namespace SpaceGuy
{
	class SpaceGuyTiledProcessor : Ebony::TiledProcessor
	{
	public:
		SpaceGuyTiledProcessor(entities::EntityPtr player) : player(player) {}
		void ParseMap(const char* filepath) override;

		void CreateTranslationFunction() override;

	
	private:
		entities::EntityPtr player;
	};
}
