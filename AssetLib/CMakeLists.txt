cmake_minimum_required(VERSION 3.12)
project(AssetLib)

add_library (assetlib STATIC
    "mesh_asset.hpp"
    "mesh_asset.cpp"
    "texture_asset.hpp"
    "texture_asset.cpp"
    "material_asset.hpp"
    "material_asset.cpp"
    "prefab_asset.hpp"
    "prefab_asset.cpp"
    "asset_load.hpp"
    "asset_loader.cpp"
)

target_include_directories(assetlib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(assetlib PRIVATE rapidjson lz4)
