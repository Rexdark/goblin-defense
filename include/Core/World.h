#pragma once

namespace sf
{
	class RenderWindow;
}

class AssetManager;
class EconomyManager;
class Level;
class Interface;

//class Enemy;
//class ObjectLayer;

class World
{
	public:

		bool load(bool newGame, float width, float height, AssetManager* assetManager);
		void unload();

		bool update(uint32_t deltaMilliseconds, sf::RenderWindow* window);
		void render(sf::RenderWindow& window);

	private:

		Level* m_level{ nullptr };
		EconomyManager* m_economyManager{ nullptr };
		Interface* m_interface{ nullptr };

		//MapLayer* m_layerTwo{ nullptr };
		//ObjectLayer* m_collisionLayer{ nullptr };

		//Enemy* m_enemy{ nullptr };
};