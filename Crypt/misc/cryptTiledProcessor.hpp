#pragma once

#include <misc/tiledProcessor.hpp>


namespace Crypt
{
	class CryptTiledProcessor : Ebony::TiledProcessor
	{
	public:
		void ParseMap(const char* filepath) override;

		void CreateTranslationFunction() override;
	};
}
