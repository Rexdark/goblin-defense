#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>

#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tmxlite/TileLayer.hpp>

#include <Utils/AssetManager.h>
#include "Core/Level.h"
#include "Utils/Constants.h"
#include "Utils/Tools.h"

void Level::init(bool newMap)
{
    mapTMXPath = std::string(MAPS_PATH) + MAIN_LEVEL_FILE_NAME + ".tmx";
    mapBinaryPath = std::string(SAVES_PATH) + MAIN_LEVEL_FILE_NAME + ".dat";

    //Load original tmx file
    if (!loadTilemap(mapTMXPath))
    {
        printf("There was an error loading the map file at %s!\n", mapTMXPath.c_str());
    }

    //Clear old save if requested.
    if (newMap)
    {
        printf("Clearing current saved map.\n");
    }

    //Check if save file does not exist.
    if (!fileExists(mapBinaryPath))
    {
        printf("Map binary file does not exist!\n");
        newMap = true;
    }

    //Create new map from tmx or just load the save file over it.
    if (newMap)
    {
        if (fileExists(mapBinaryPath))
        {
            std::filesystem::remove(mapBinaryPath);
        }
        createBinaryFile();
    }
    else
    {
        loadBinaryFile();
    }
}

Level::~Level()
{
    deleteTilemapVector3();
    unloadTilemap();
}

void Level::createBinaryFile()
{
    //printf("Creating new map binary file at %s.\n", mapBinaryPath.c_str());

    saveToFile();
}

void Level::loadBinaryFile()
{
    //printf("Found saved map file at %s.\n", mapBinaryPath.c_str());

    loadFromFile();

}

bool Level::loadTilemap(const std::string mapPath)
{
    m_tilemap = new tmx::Map();

    if (!m_tilemap->load(mapPath)) {
        printf("Error loading map file at %s.\n", mapPath.c_str());
        return false;
    }

    int layers = getMapLayerNumber();

    m_mapLayers = new MapLayer * [layers];

    for (int layer = 0; layer < layers; ++layer)
    {
        m_mapLayers[layer] = new MapLayer (*m_tilemap, layer);
    }

    return true;    
}

void Level::populateTilemapLayers()
{
    //printf("Poulating tilemap with size %d, %d.\n", getRows(), getCols());

    for (int layer = 0; layer < getMapLayerNumber(); ++layer)
    {
        MapLayer* currentMapLayer = m_mapLayers[layer];

        //printf("Populating layer %d.\n", layer);

        const int numRows = getRows(layer);

        for (int row = 0; row < numRows; ++row)
        {
            const int numCols = getCols(layer, row);

            for (int col = 0; col < numCols; ++col)
            {
                tmx::TileLayer::Tile tile = { (*m_tilemap_vector3)[layer][row][col], 0 };
                currentMapLayer->setTile(col, row, tile, true); //For some reason setTile wants first the columns and then the rows.
                //printf("Setting tile on [%d][%d] with value %d.\n", row, col, tile.ID);
            }
        }
    }
}

void Level::unloadTilemap()
{
    if (m_tilemap != nullptr)
    {
        delete m_tilemap;
        m_tilemap = nullptr;
    }

    if (m_mapLayers != nullptr)
    {
        delete[] m_mapLayers;
        m_mapLayers = nullptr;
    }
}

void Level::generateTilemapVector3(int layers, int rows, int cols, int defaultValue)
{
    m_tilemap_vector3 = new std::vector<std::vector<std::vector<uint32_t>>>(layers, std::vector<std::vector<uint32_t>>(rows, std::vector<uint32_t>(cols, defaultValue)));
}

void Level::populateTilemapVector3()
{
    const auto& layers = m_tilemap->getLayers();    

    for (int layer = 0; layer < getMapLayerNumber(); ++layer) 
    {
        const auto& tiles = layers[layer]->getLayerAs<tmx::TileLayer>().getTiles();

        const int numRows = getRows(layer);

        for (int row = 0; row < numRows; ++row) 
        {
            const int numCols = getCols(layer, row);

            for (int col = 0; col < numCols; ++col) 
            {
                int tileIndex = row * numCols + col;
                const auto& tile = tiles[tileIndex];
                (*m_tilemap_vector3)[layer][row][col] = tile.ID;
            }
        }
    }
}

void Level::deleteTilemapVector3()
{
    if (m_tilemap_vector3 != nullptr)
    {
        delete m_tilemap_vector3;
        m_tilemap_vector3 = nullptr;
    }
}

void Level::saveToFile()
{
    printf("Saving map to file at %s.\n", mapBinaryPath.c_str());

    generateTilemapVector3(getMapLayerNumber(), getMapRowNumber(), getMapColNumber());
    populateTilemapVector3();

    std::filesystem::path filePath(mapBinaryPath);

    //Just in case folder might have been deleted by user
    if (!std::filesystem::exists(filePath.parent_path())) {
        std::filesystem::create_directories(filePath.parent_path());
    }

    std::ofstream file(mapBinaryPath, std::ios::binary);
    if (!file.is_open()) {
        printf("Failed to open file at %s.\n", mapBinaryPath.c_str());
        return;
    }

    uint32_t  layers = getLayers();
    uint32_t  rows = getRows();
    uint32_t  cols = getCols();

    //Is the entire 3d Vector (std::vector<std::vector<std::vector<uint32_t>>>) stored in a contiguous block of memory??
    //file.write(reinterpret_cast<const char*>((*m_tilemap_vector3).data()), layers * rows * cols * sizeof(int));

    file.write(reinterpret_cast<const char*>(&layers), sizeof(layers));
    file.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
    file.write(reinterpret_cast<const char*>(&cols), sizeof(cols));

    for (uint32_t layer = 0; layer < layers; ++layer)
    {
        for (uint32_t row = 0; row < rows; ++row)
        {
            for (uint32_t col = 0; col < cols; ++col)
            {
                int tileValue = (*m_tilemap_vector3)[layer][row][col];
                file.write(reinterpret_cast<const char*>(&tileValue), sizeof(tileValue));

                //printf("Saved value of tile [%d][%d][%d] as %d.\n", layer, row, col, tileValue);
            }
        }
    }

    file.close();

    deleteTilemapVector3();
}

void Level::loadFromFile()
{
    printf("Loading map from file at %s.\n", mapBinaryPath.c_str());

    std::filesystem::path filePath(mapBinaryPath);

    if (!std::filesystem::exists(filePath)) {
        printf("File does not exist at %s!\n", mapBinaryPath.c_str());
        return;
    }

    std::ifstream file(mapBinaryPath, std::ios::binary);
    if (!file.is_open()) {
        printf("Failed to open file at %s.\n", mapBinaryPath.c_str());
        return;
    }

    uint32_t  layers = 0;
    uint32_t  rows = 0;
    uint32_t  cols = 0;

    file.read(reinterpret_cast<char*>(&layers), sizeof(layers));
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

    if (getMapLayerNumber() == layers && getMapRowNumber() == rows && getMapColNumber() == cols)
    {
        generateTilemapVector3(getMapLayerNumber(), getMapRowNumber(), getMapColNumber());
    }
    else
    {
        printf("Saved map has a different size than the original!\n");
        return;
    }

    for (uint32_t layer = 0; layer < layers; ++layer)
    {
        for (uint32_t row = 0; row < rows; ++row)
        {
            for (uint32_t col = 0; col < cols; ++col)
            {
                uint32_t  tileValue = 0;
                file.read(reinterpret_cast<char*>(&tileValue), sizeof(tileValue));

                (*m_tilemap_vector3)[layer][row][col] = tileValue;

                //printf("Loaded value of tile [%d][%d][%d] as %d.\n", layer, row, col, tileValue);
            }
        }
    }

    file.close();

    populateTilemapLayers();

    deleteTilemapVector3();
}

void Level::render(sf::RenderWindow& window)
{
    for (int layer = 0; layer < getMapLayerNumber(); ++layer)
    {
        window.draw(*m_mapLayers[layer]);
    }
}

//NEEDS UPDATE TO WORK WITH TMXLITE MAPS OR OVER THE VECTOR3 AND THEN AN UPDATE METHOD !!

int Level::getMapLayerNumber()
{
    const auto& layers = m_tilemap->getLayers();
    return layers.size();
}

int Level::getMapRowNumber()
{
    return m_tilemap->getTileCount().y;
}

int Level::getMapColNumber()
{
    return m_tilemap->getTileCount().x;
}

int Level::getTile(int layer, int row, int col) const 
{
    return (*m_tilemap_vector3)[layer][row][col];
}

void Level::setTile(int layer, int row, int col, int value)
{
    (*m_tilemap_vector3)[layer][row][col] = value;
}

int Level::getLayers()
{
    return (*m_tilemap_vector3).size();
}

int Level::getRows(int layer) const
{
    return (*m_tilemap_vector3)[layer].size();
}

int Level::getCols( int layer, int row) const
{
    return (*m_tilemap_vector3)[layer][row].size();
}