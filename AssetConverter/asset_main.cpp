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
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"

#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"


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
	texInfo.textureFormat = 4;
	texInfo.originalFile = input.string();
	texInfo.compressionMode = assets::CompressionMode::LZ4;
	assets::AssetFile newImage = assets::pack_texture(&texInfo, pixels);

	stbi_image_free(pixels);

	assets::save_binaryfile(output.string().c_str(), newImage);
	
	return true;
}

int main(int argc, char* argv[])
{
	//if (argc < 2)
	//{
	//	std::cout << "You need to put the path to the info file";
	//	return -1;
	//}
	std::filesystem::path directoryTextures = "../../../Graphics/textures";
	std::filesystem::path directoryModels = "../../../Graphics/models";

	std::filesystem::path cwd = std::filesystem::current_path();
	std::cout << cwd << std::endl;

	std::cout << "Loading asset directory at " << directoryTextures << std::endl;

	for (auto& p : std::filesystem::recursive_directory_iterator(directoryTextures))
	{
		std::cout << "File: " << p << std::endl;

		if (p.path().extension() == ".png" || p.path().extension() == ".jpg")
		{
			std::cout << "Found a texture" << std::endl;

			auto newpath = p.path();
			newpath.replace_extension(".tx");
			convert_image(p.path(), newpath);
		}
	}

	std::cout << "Loading asset directory at " << directoryModels << std::endl;

	for (auto& p : std::filesystem::recursive_directory_iterator(directoryModels))
	{
		std::cout << "File: " << p << std::endl;

		if (p.path().extension() == ".obj")
		{
			std::cout << "Found a model" << std::endl;

			//auto newpath = p.path();
			//newpath.replace_extension(".mesh");
			//convert_image(p.path(), newpath);
		}
	}


	return 0;
}