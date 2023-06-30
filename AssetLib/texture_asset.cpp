#include "texture_asset.hpp"
#include <iostream>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "lz4.h"

assets::TextureFormat parse_format(const char* f)
{
	if (strcmp(f, "RGBA8") == 0)
	{
		return assets::TextureFormat::RGBA8;
	}
	else
	{
		return assets::TextureFormat::Unknown;
	}
}

assets::AssetFile assets::pack_texture(assets::TextureInfo* info, void* pixelData)
{
	rapidjson::Document document;
	document.SetObject();

	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	size_t sz = allocator.Size();

	document.AddMember("format", "RGBA8", allocator);
	document.AddMember("width", info->pixelSize[0], allocator);
	document.AddMember("height", info->pixelSize[1], allocator);
	document.AddMember("buffer_size", info->textureSize, allocator);
	document.AddMember("original_file", *(info->originalFile).data(), allocator);

	// Core file header
	AssetFile file;
	file.type[0] = 'T';
	file.type[1] = 'E';
	file.type[2] = 'X';
	file.type[3] = 'I';
	file.version = 1;

	// Compress buffer into blob
	// Find the maximum data needed for the compression
	int compressStaging = LZ4_compressBound(static_cast<int>(info->textureSize));

	// Make sure the blob storage has enough size for the maximum
	file.binaryBlob.resize(compressStaging);

	// This is like a memcpy, except it compresses the data and returns the compressed size
	int compressedSize = LZ4_compress_default((const char*)pixelData, file.binaryBlob.data(), static_cast<int>(info->textureSize), compressStaging);

	// We can now resize the blob down to the final compressed size.
	file.binaryBlob.resize(compressedSize);

	document.AddMember("compression", "LZ4", allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	file.json = buffer.GetString();;

	return file;
}



assets::TextureInfo assets::read_texture_info(AssetFile* file)
{
	TextureInfo info{};
	rapidjson::Document document;

	document.Parse(file->json.data());

	std::string formatString = document["format"].GetString();
	info.textureFormat = parse_format(formatString.c_str());

	std::string compressionString = document["compression"].GetString();
	info.compressionMode = parse_compression(compressionString.c_str());

	info.pixelSize[0] = document["width"].GetUint();
	info.pixelSize[1] = document["height"].GetUint();
	info.textureSize = document["buffer_size"].GetUint64();
	info.originalFile = document["original_file"].GetString();

	return info;
}




void assets::unpack_texture(TextureInfo* info, const char* sourceBuffer, size_t sourceSize, char* destination)
{
	if (info->compressionMode == CompressionMode::LZ4)
	{
		LZ4_decompress_safe(sourceBuffer, destination, static_cast<int>(sourceSize), static_cast<int>(info->textureSize));
	}
	else 
	{
		memcpy(destination, sourceBuffer, sourceSize);
	}
}