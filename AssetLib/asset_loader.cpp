#include <asset_load.hpp>

#include <fstream>
#include <iostream>

bool assets::save_binaryfile(const char* path, const assets::AssetFile& file)
{
	std::ofstream outFile;
	outFile.open(path, std::ios::binary | std::ios::out);
	if (!outFile.is_open())
	{
		std::cout << "Error when trying to write file: " << path << std::endl;
		return false;
	}

	outFile.write(file.type, sizeof(char) * 4); // Will either be TEXI or MESH (Texture or mesh)
	uint32_t version = file.version; // Versioning is super helpful to give an error if you changed the version of your file formats

	// Version
	outFile.write((const char*)&version, sizeof(uint32_t));

	// Json length
	uint32_t length = static_cast<uint32_t>(file.json.size());
	outFile.write((const char*)&length, sizeof(uint32_t));

	// Blob length
	uint32_t blobLength = static_cast<uint32_t>(file.binaryBlob.size());
	outFile.write((const char*)&blobLength, sizeof(uint32_t));

	// Json stream
	outFile.write(file.json.data(), length);

	// Blob data
	outFile.write(file.binaryBlob.data(), file.binaryBlob.size());

	outFile.close();

	return true;
}

bool assets::load_binaryfile(const char* path, assets::AssetFile& outputFile)
{
	std::ifstream inFile;
	inFile.open(path, std::ios::binary);

	if (!inFile.is_open()) return false;

	// Move file cursor to the beginning
	inFile.seekg(0);

	inFile.read(outputFile.type, sizeof(char) * 4);
	inFile.read((char*)&outputFile.version, sizeof(uint32_t)); // In the future, do a versioning check after reading this
	
	uint32_t jsonLen = 0;
	inFile.read((char*)&jsonLen, sizeof(uint32_t));

	uint32_t blobLen = 0;
	inFile.read((char*)&blobLen, sizeof(uint32_t));


	outputFile.json.resize(jsonLen);
	inFile.read((char*)outputFile.json.data(), jsonLen);

	outputFile.binaryBlob.resize(blobLen);
	inFile.read(outputFile.binaryBlob.data(), blobLen);

	return true;
}


assets::CompressionMode assets::parse_compression(const char* f)
{
	if (strcmp(f, "LZ4") == 0)
	{
		return assets::CompressionMode::LZ4;
	}
	else
	{
		return assets::CompressionMode::None;
	}
}


