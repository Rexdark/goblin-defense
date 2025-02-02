#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SFML/System.hpp>

namespace sf
{
    class RenderWindow;
}

namespace tmx
{
    class Map;
}

class MapLayer;

class Level
{
    public:

        ~Level();

        struct Tile
        {
            int layer = 0;
            int row = 0;
            int col = 0;
            uint32_t value = 0;
        };

        void init(bool newTileset = false);

        Tile getTile(int layer, int row, int col) const;
        void setTile(Tile tile, bool array = false);

        int getLayers();
        int getRows(int layer = 0) const;
        int getCols(int layer = 0, int row = 0) const;
        std::vector<Level::Tile> getEnemySpawnTiles();

        sf::Vector2i getTilebyCoordinates(int width, int height);
        sf::Vector2f getTileCoordinates(int row, int col);

        std::vector<std::vector<uint32_t>> getPathSearchMapLayer();

        void update(sf::RenderWindow* window);
        void render(sf::RenderWindow& window, bool terrainLayer);

        void saveToFile();
        void loadFromFile();

        bool getBuildMode() const;
        void setBuildMode(bool enable = true);

        void renderBuildinginBuildMode(sf::RenderWindow* window, bool build = false);

        std::vector <sf::Vector2f> getTowerCoordinates();

    private:

        bool m_buildModeEnabled = false;
        bool m_waitForBuilding = false;

        const std::vector<uint32_t> unbuildableTiles = { 55, 56, 57, 67, 68, 69, 79, 80, 81, 91, 92, 103, 104};
        const std::vector<uint32_t> otherBuildingTiles = { 158, 153, 164, 165, 176, 177, 188,  189 };
        int m_selectedBuildingId = 0; //Preparation to enable different buildings in the future

        std::vector<Level::Tile> enemySpawnTiles;

        std::string mapTMXPath = "";
        std::string mapBinaryPath = "";

        tmx::Map* m_tilemap{ nullptr };
        std::vector<std::vector<std::vector<uint32_t>>>* m_tilemap_vector3{ nullptr };

        MapLayer** m_mapLayers{ nullptr };
        int m_layerCount = 0;

        int tileSize = 0;

        void createEnemySpawnTilePool();

        void createBinaryFile();
        void loadBinaryFile();

        void generateTilemapVector3(int layers, int rows, int cols, int defaultValue = 0);
        void populateTilemapVector3();
        void deleteTilemapVector3();

        bool loadTilemap(const std::string path);
        void populateTilemapLayers(int layer = -1);
        void unloadTilemap();

        int getMapLayerNumber();
        int getMapRowNumber();
        int getMapColNumber();

        sf::Vector2i getTileUnderMouse(sf::RenderWindow* window);
        void renderSelectedBuilding(sf::Vector2i tile, bool build);
        void clearPreviewLayer();

        bool checkIfCanBuild(sf::Vector2i tile);

        void setTileArray(std::vector<Tile> tileArray, Tile destinationTile);
        std::vector<Tile> copyBuildingValues(std::vector<Tile>* tileArray, Tile originTile, Tile destinationTile, sf::Vector2i buildingSize);
};