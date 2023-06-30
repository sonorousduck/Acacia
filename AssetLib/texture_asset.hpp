#pragma once
#include "asset_load.hpp"



namespace assets 
{
	enum class TextureFormat : uint32_t
	{
		Unknown = 0,
		RGBA8
	};

	struct TextureInfo
	{
		uint64_t textureSize;
		TextureFormat textureFormat;
		CompressionMode compressionMode;

		uint32_t pixelSize[3];
		std::string originalFile;
	};

	// Parses the texture metadata from an asset file
	TextureInfo read_texture_info(AssetFile* file);

	void unpack_texture(TextureInfo* info, const char* sourceBuffer, size_t sourceSize, char* destination);

	AssetFile pack_texture(TextureInfo* info, void* pixelData);

}