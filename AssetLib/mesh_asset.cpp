#include "mesh_asset.hpp"
#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "lz4.h"
#include <complex>


assets::VertexFormat parse_format(const char* f)
{
	if (strcmp(f, "PNCV_F32") == 0)
	{
		return assets::VertexFormat::PNCV_F32;
	}
	else if (strcmp(f, "P32N8C8V16") == 0)
	{
		return assets::VertexFormat::P32N8C8V16;
	}
	else
	{
		return assets::VertexFormat::Unknown;
	}
}

assets::MeshInfo assets::read_mesh_info(AssetFile* file)
{
	MeshInfo info{};

	rapidjson::Document document;
	document.Parse(file->json);
	
	info.vertexBufferSize = document["vertex_buffer_size"].GetUint64();
	info.indexBufferSize = document["index_buffer_size"].GetUint64();
	info.indexSize = (uint8_t)document["index_size"].GetUint();
	info.originalFile = document["original_file"].GetString();

	std::string compressionString = document["compression"].GetString();
	info.compressionMode = parse_compression(compressionString.c_str());

	std::vector<float> boundsData{};
	boundsData.reserve(7);
	
	for (auto& e : document["bounds"].GetArray())
	{
		boundsData.push_back(e.GetFloat());
	}

	info.bounds.origin[0] = boundsData[0];
	info.bounds.origin[1] = boundsData[1];
	info.bounds.origin[2] = boundsData[2];

	info.bounds.radius = boundsData[3];

	info.bounds.extents[0] = boundsData[4];
	info.bounds.extents[1] = boundsData[5];
	info.bounds.extents[2] = boundsData[6];

	std::string vertexFormat = document["vertex_format"].GetString();
	info.vertexFormat = parse_format(vertexFormat.c_str());

	return info;
}

assets::AssetFile assets::pack_mesh(MeshInfo* meshInfo, char* vertexData, char* indexData)
{
	AssetFile file;
	file.type[0] = 'M';
	file.type[1] = 'E';
	file.type[2] = 'S';
	file.type[3] = 'H';
	file.version = 1;


	rapidjson::Document document;
	document.SetObject();

	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	size_t sz = allocator.Size();


	if (meshInfo->vertexFormat == VertexFormat::P32N8C8V16) {
		document.AddMember("vertex_format", "P32N8C8V16", allocator);

	}
	else if (meshInfo->vertexFormat == VertexFormat::PNCV_F32)
	{
		document.AddMember("vertex_format", "PNCV_F32", allocator);
	}

	document.AddMember("vertex_buffer_size", meshInfo->vertexBufferSize, allocator);
	document.AddMember("index_buffer_size", meshInfo->indexBufferSize, allocator);
	document.AddMember("index_size", meshInfo->indexSize, allocator);
	document.AddMember("original_file", meshInfo->originalFile, allocator);

	std::vector<float> boundsData{};
	boundsData.resize(7);

	boundsData[0] = meshInfo->bounds.origin[0];
	boundsData[1] = meshInfo->bounds.origin[1];
	boundsData[2] = meshInfo->bounds.origin[2];

	boundsData[3] = meshInfo->bounds.radius;

	boundsData[4] = meshInfo->bounds.extents[0];
	boundsData[5] = meshInfo->bounds.extents[1];
	boundsData[6] = meshInfo->bounds.extents[2];

	rapidjson::Value bounds(rapidjson::kArrayType);


	for (const auto& bound : boundsData)
	{
		rapidjson::Value value{};
		value.SetFloat(bound);
		bounds.PushBack(value, allocator);
	}

	document.AddMember("bounds", bounds, allocator);


	size_t fullsize = meshInfo->vertexBufferSize + meshInfo->indexBufferSize;

	std::vector<char> merged_buffer;
	merged_buffer.resize(fullsize);

	//copy vertex buffer
	memcpy(merged_buffer.data(), vertexData, meshInfo->vertexBufferSize);

	//copy index buffer
	memcpy(merged_buffer.data() + meshInfo->vertexBufferSize, indexData, meshInfo->indexBufferSize);


	//compress buffer and copy it into the file struct
	size_t compressStaging = LZ4_compressBound(static_cast<int>(fullsize));

	file.binaryBlob.resize(compressStaging);

	int compressedSize = LZ4_compress_default(merged_buffer.data(), file.binaryBlob.data(), static_cast<int>(merged_buffer.size()), static_cast<int>(compressStaging));
	file.binaryBlob.resize(compressedSize);

	document.AddMember("compression", "LZ4", allocator);
	
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	file.json = buffer.GetString();
	return file;
}

void assets::unpack_mesh(MeshInfo* info, const char* sourceBuffer, size_t sourceSize, char* vertexBuffer, char* indexBuffer)
{
	//decompressing into temporal vector. TODO: streaming decompress directly on the buffers
	std::vector<char> decompressedBuffer;
	decompressedBuffer.resize(info->vertexBufferSize + info->indexBufferSize);

	LZ4_decompress_safe(sourceBuffer, decompressedBuffer.data(), static_cast<int>(sourceSize), static_cast<int>(decompressedBuffer.size()));

	//copy vertex buffer
	memcpy(vertexBuffer, decompressedBuffer.data(), info->vertexBufferSize);

	//copy index buffer
	memcpy(indexBuffer, decompressedBuffer.data() + info->vertexBufferSize, info->indexBufferSize);
}

assets::MeshBounds assets::calculateBounds(Vertex_f32_PNCV* vertices, size_t count)
{
	MeshBounds bounds{};

	float min[3] = { std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max() };
	float max[3] = { std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min() };

	for (int i = 0; i < count; i++) {
		min[0] = std::min(min[0], vertices[i].position[0]);
		min[1] = std::min(min[1], vertices[i].position[1]);
		min[2] = std::min(min[2], vertices[i].position[2]);

		max[0] = std::max(max[0], vertices[i].position[0]);
		max[1] = std::max(max[1], vertices[i].position[1]);
		max[2] = std::max(max[2], vertices[i].position[2]);
	}

	bounds.extents[0] = (max[0] - min[0]) / 2.0f;
	bounds.extents[1] = (max[1] - min[1]) / 2.0f;
	bounds.extents[2] = (max[2] - min[2]) / 2.0f;

	bounds.origin[0] = bounds.extents[0] + min[0];
	bounds.origin[1] = bounds.extents[1] + min[1];
	bounds.origin[2] = bounds.extents[2] + min[2];

	//go through the vertices again to calculate the exact bounding sphere radius
	float r2 = 0;
	for (int i = 0; i < count; i++) {

		float offset[3];
		offset[0] = vertices[i].position[0] - bounds.origin[0];
		offset[1] = vertices[i].position[1] - bounds.origin[1];
		offset[2] = vertices[i].position[2] - bounds.origin[2];

		//pithagoras
		float distance = offset[0] * offset[0] + offset[1] * offset[1] + offset[2] * offset[2];
		r2 = std::max(r2, distance);
	}

	bounds.radius = std::sqrt(r2);

	return bounds;
}