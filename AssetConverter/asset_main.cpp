#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>


#include "lz4.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "tiny_obj_loader.h"

#include "asset_load.hpp"
#include "texture_asset.hpp"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_USE_RAPIDJSON
#define TINYGLTF_NO_INCLUDE_RAPIDJSON
#include "tiny_gltf.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"



bool convert_image(const std::filesystem::path& input, const std::filesystem::path& output)
{
	int texWidth = 0, texHeight = 0, texChannels = 0;

	stbi_uc* pixels = stbi_load(input.string().c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	if (!pixels)
	{
		std::cout << "Failed to load texture file " << input << std::endl;
		return false;
	}

	int texture_size = texWidth * texHeight * 4;

	assets::TextureInfo texInfo{};
	texInfo.textureSize = texture_size;
	texInfo.pixelSize[0] = texWidth;
	texInfo.pixelSize[1] = texHeight;
	texInfo.textureFormat = assets::TextureFormat::RGBA8;
	texInfo.originalFile = input.string();
	assets::AssetFile newImage = assets::pack_texture(&texInfo, pixels);

	stbi_image_free(pixels);

	assets::save_binaryfile(output.string().c_str(), newImage);

	return true;
}