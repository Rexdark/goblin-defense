#pragma once

namespace sf
{
	class RenderWindow;
}

class AssetManager;
class EconomyManager;
class Level;
class Interface;
class EnemyManager;

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
		EnemyManager* m_enemyManager{ nullptr };

		sf::RenderWindow* m_window{ nullptr };

		//ObjectLayer* m_collisionLayer{ nullptr };
};