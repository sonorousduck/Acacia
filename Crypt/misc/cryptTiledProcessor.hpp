#pragma once

#include <misc/tiledProcessor.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <components/rigidbodyComponent.hpp>
#include <components/collider.hpp>
#include <misc/resourceManager.hpp>


namespace Crypt
{
	class CryptTiledProcessor : Ebony::TiledProcessor
	{
	public:
		void ParseMap(const char* filepath, std::function<void(entities::EntityPtr)> AddEntity) override;

		void CreateTranslationFunction() override;
	};
}
