#pragma once

#include <cstdint>
#include <string>
#include <vector>

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

        void init(bool newTileset = false);

        int getTile(int layer, int row, int col) const;
        void setTile(int layer, int row, int col, int value);

        int getLayers();
        int getRows(int layer = 0) const;
        int getCols(int layer = 0, int row = 0) const;

        void render(sf::RenderWindow& window);

        void saveToFile();
        void loadFromFile();

    private:

        std::string mapTMXPath = "";
        std::string mapBinaryPath = "";

        tmx::Map* m_tilemap{ nullptr };
        std::vector<std::vector<std::vector<uint32_t>>>* m_tilemap_vector3{ nullptr };

        MapLayer** m_mapLayers{ nullptr };

        int tileSize = 0;

        void createBinaryFile();
        void loadBinaryFile();

        void generateTilemapVector3(int layers, int rows, int cols, int defaultValue = 0);
        void populateTilemapVector3();
        void deleteTilemapVector3();

        bool loadTilemap(const std::string path);
        void populateTilemapLayers();
        void unloadTilemap();

        int getMapLayerNumber();
        int getMapRowNumber();
        int getMapColNumber();
};