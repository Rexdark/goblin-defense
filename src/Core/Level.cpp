#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>

#include <Render/SFMLOrthogonalLayer.h>
#include <SFML/Graphics.hpp>
#include <tmxlite/TileLayer.hpp>

#include <Utils/AssetManager.h>
#include "Core/Level.h"
#include "Utils/Constants.h"
#include "Utils/Tools.h"

#include <thread>

void Level::init(bool newMap)
{
    mapTMXPath = std::string(MAPS_PATH) + MAIN_LEVEL_FILE_NAME + ".tmx";
    mapBinaryPath = std::string(SAVES_PATH) + MAIN_LEVEL_FILE_NAME + ".dat";

    if (!loadTilemap(mapTMXPath))
    {
        printf("There was an error loading the map file at %s!\n", mapTMXPath.c_str());
    }

    if (newMap)
    {
        printf("Clearing current saved map.\n");
    }

    if (!fileExists(mapBinaryPath))
    {
        printf("Map binary file does not exist!\n");
        newMap = true;
    }

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

void Level::createEnemySpawnTilePool()
{
    enemySpawnTiles.clear();

    int layer = 1; //Terrain layer

    if (m_tilemap_vector3->empty() || m_tilemap_vector3[0].empty())
    {
        printf("No tilemap!!\n");
        return;
    }


    for (int row = 0; row < (*m_tilemap_vector3)[layer].size(); ++row)
    {
        for (int col = 0; col < (*m_tilemap_vector3)[layer][row].size(); ++col)
        {
            if (col == 0 && (row > 5 && row < 15))
            {
                Tile tile = getTile(layer, row, col);

                if (tile.value == 1)
                {
                    //printf("Adding tile [%d][%d] to the spawn pool!\n", col, row);
                    enemySpawnTiles.push_back(tile);
                }
            }
        }
    }
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

void Level::populateTilemapLayers(int selected_layer)
{
    if (selected_layer != -1)
    {
        MapLayer* currentMapLayer = m_mapLayers[selected_layer];

        const int numRows = getRows(selected_layer);

        for (int row = 0; row < numRows; ++row)
        {
            const int numCols = getCols(selected_layer, row);

            for (int col = 0; col < numCols; ++col)
            {
                if (currentMapLayer->getTile(col, row).ID != (*m_tilemap_vector3)[selected_layer][row][col])
                {
                    tmx::TileLayer::Tile tile = { (*m_tilemap_vector3)[selected_layer][row][col], 0 };
                    currentMapLayer->setTile(col, row, tile, true);
                }
            }
        }

        return;
    }

    for (int layer = 0; layer < getMapLayerNumber(); ++layer)
    {
        MapLayer* currentMapLayer = m_mapLayers[layer];

        const int numRows = getRows(layer);

        for (int row = 0; row < numRows; ++row)
        {
            const int numCols = getCols(layer, row);

            for (int col = 0; col < numCols; ++col)
            {
                tmx::TileLayer::Tile tile = { (*m_tilemap_vector3)[layer][row][col], 0 };
                currentMapLayer->setTile(col, row, tile, true);
                //printf("Layer %d, tile [%d][%d] has now the value %d.\n", layer, col, row, currentMapLayer->getTile(col, row).ID);
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

    //deleteTilemapVector3();
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

    //deleteTilemapVector3();
}

void Level::update(sf::RenderWindow* window)
{
    if (m_buildModeEnabled)
    {
        renderBuildinginBuildMode(window);
    }
}

void Level::render(sf::RenderWindow& window)
{
    for (int layer = 0; layer < getMapLayerNumber(); ++layer)
    {
        if (layer == 3 && !m_buildModeEnabled)
        {
            continue;
        }
        window.draw(*m_mapLayers[layer]);
    }
}

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

Level::Tile Level::getTile(int layer, int row, int col) const 
{
    Tile tile = {};

    tile.layer = layer;
    tile.row = row;
    tile.col = col;
    tile.value = (*m_tilemap_vector3)[layer][row][col];

    return tile;
}

void Level::setTile(Tile tile, bool array)
{
    if ((m_tilemap_vector3 != nullptr && m_tilemap_vector3->size() > tile.layer) &&
    (tile.col >= 0 && tile.col < (*m_tilemap_vector3)[tile.layer].size()) &&
    (tile.row >= 0 && tile.row < (*m_tilemap_vector3)[tile.layer][tile.col].size()))
    {
        //printf("Setting Tile!\n");
        (*m_tilemap_vector3)[tile.layer][tile.col][tile.row] = tile.value;
    }

    if (!array)
    {
        populateTilemapLayers(tile.layer);
    }
}

void Level::setTileArray(std::vector<Tile> tileArray, Tile destinationTile)
{
    for (Tile tile : tileArray) //Get start
    {
        //printf("Setting in layer %d and tile [%d][%d] the value %d.\n", tile.layer, tile.row, tile.col, tile.value);
        setTile(tile, true);
    }

    //printf("\n============\n");

    populateTilemapLayers(tileArray[0].layer);
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

std::vector<Level::Tile> Level::getEnemySpawnTiles()
{
    createEnemySpawnTilePool();
    return enemySpawnTiles;
}

sf::Vector2i Level::getTilebyCoordinates(int height, int width)
{
    int tile_size = m_tilemap->getTileSize().x; //We asume square tiles since nobody's going to use rectangular ones

    int col = std::round(height / tile_size);
    int row = std::round(width / tile_size);
    
    col = std::max(0, std::min(col, getCols() - 2));
    row = std::max(0, std::min(row, getRows() - 1));

    sf::Vector2i vector = { row, col };

    //printf("Mouse is over tile [%d][%d]\n", vector.x, vector.y);

    return vector;
}

sf::Vector2i Level::getTileCoordinates(int row, int col)
{
    int tile_size = m_tilemap->getTileSize().x; //We asume square tiles since nobody's going to use rectangular ones

    int height = (tile_size * row) + tile_size / 2;

    int witdth = (tile_size * col) + tile_size / 2;

    sf::Vector2i vector = { height, witdth };

    return vector;
}

MapLayer* Level::getPathSearchMapLayer()
{
    MapLayer* pathMapLayer = new MapLayer(*m_tilemap, 1);
    MapLayer& mapLayer2 = (*m_mapLayers)[2];

    sf::FloatRect bounds = (*m_mapLayers)[1].getGlobalBounds();
    int width = bounds.width;
    int height = bounds.height;

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            if (mapLayer2.getTile(x, y).ID != 0)
            {
                pathMapLayer->setTile(x, y, mapLayer2.getTile(x, y), true);
            }
        }
    }

    return pathMapLayer;
}

sf::Vector2i Level::getTileUnderMouse(sf::RenderWindow* window)
{
    if (!window)
    {
        return { 0, 0 }; // Sometimes for some reson it try to get the mouse before the window exists.
    }

    sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

    sf::Vector2i tile = getTilebyCoordinates(mousePos.x, mousePos.y);

    return tile;
}

bool Level::getBuildMode() const
{
    return m_buildModeEnabled;
}

void Level::setBuildMode(bool enable)
{
    m_buildModeEnabled = enable;
}

void Level::renderBuildinginBuildMode(sf::RenderWindow* window, bool build)
{
    sf::Vector2i tile = getTileUnderMouse(window);

    if (checkIfCanBuild(tile)) //No builing past the village or over forbbiden tiles
    {
        renderSelectedBuilding(tile, build);
    }
    else
    {
        clearPreviewLayer();
    }
}

bool Level::checkIfCanBuild(sf::Vector2i tile)
{
    sf::Vector2i buildingSize = { 4, 2 }; //This won't be need when loading building settings from file

    if (tile.y > 23) //No building over the village or past it.
    {
        return false;
    }

    for (uint32_t value : unbuildableTiles) //No building over cliffs
    {
        if (getTile(1, tile.x, tile.y).value == value || getTile(1, tile.x, tile.y + 1).value == value) 
        {
            return false;
        }

        
    }

    for (uint32_t value : otherBuildingTiles) //No building over other buildings to avoid texture clipping
    {
        for (int i = tile.x ; i > (tile.x - buildingSize.x); --i)
        {
            for (int j = tile.y ; j < (tile.y + buildingSize.y); ++j)
            {
                if ((i < 0 || i > getRows()) || (j < 0 || j > getCols()))
                {
                    return false;
                }

                if (getTile(2, i, j).value == value)
                {
                    return false;
                }
            }
        }      
    }

    return true;
}

void Level::renderSelectedBuilding(sf::Vector2i tile, bool build)
{
    Level::Tile originTile = {};
    Level::Tile destinationTile = { build ? 2 : 3, tile.x, tile.y }; //Layer 3 is preview and Layer 2 is buildings
    std::vector<Level::Tile> tileArray = {};
    sf::Vector2i buildingSize = {};

    clearPreviewLayer();

    switch (m_selectedBuildingId) //Ideally this should load from file
    {
        case 0:

            originTile = { 1, 3, 0 }; //Coordinates of Tower, Layer 1 is building database

            buildingSize = { 4, 2 };

            tileArray.resize(buildingSize.x * buildingSize.y);

            tileArray = copyBuildingValues(&tileArray, originTile, destinationTile, buildingSize);

            break;

        default:

            printf("Selected building id not found!");
            break;
    }

    setTileArray(tileArray, destinationTile);

    if (build)
    {
        m_waitForBuilding = false;
    }
}

void Level::clearPreviewLayer()
{
    if (m_tilemap_vector3 != nullptr && m_tilemap_vector3->size() > 3)
    {
        auto& previewLayer = (*m_tilemap_vector3)[3];
        for (auto& row : previewLayer)
        {
            for (auto& tile : row)
            {
                tile = 0;
            }
        }
    }
    else
    {
        printf("Layer 3 does not exist or tilemap vector is null!\n");
    }
}

std::vector<Level::Tile> Level::copyBuildingValues(std::vector<Tile>* tileArray, Tile originTile, Tile destinationTile, sf::Vector2i buildingSize)
{
    int buildingHeight = buildingSize.x;
    int buildingWidth = buildingSize.y;

    for (int i = 0; i > (0 - buildingHeight); --i)
    {
        for (int j = 0; j < buildingWidth; ++j)
        {
            int sourceRow = originTile.row + i;
            int sourceCol = originTile.col + j;
            uint32_t sourceValue = (*m_tilemap_vector3)[originTile.layer-1][sourceRow][sourceCol];

            int destRow = destinationTile.row + i;
            int destCol = destinationTile.col + j;
            Level::Tile tempTile = { destinationTile.layer, destCol, destRow, sourceValue }; //Had to invert because coordinates are col/row not row/col. Maybe later could rewrite the struct but there's no time now.

            (*tileArray)[(-i) * buildingWidth + j] = tempTile;
        }
    }

    return *tileArray;
}

