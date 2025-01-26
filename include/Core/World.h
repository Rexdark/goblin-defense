#pragma once

#include <cstdint>

namespace sf
{
	class RenderWindow;
}

class Level;

//class Enemy;
//class ObjectLayer;

class World
{
	public:

		~World();

		bool load(bool newGame = false);
		void unload();

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

	private:

		Level* m_level{ nullptr };
		//MapLayer* m_layerTwo{ nullptr };
		//ObjectLayer* m_collisionLayer{ nullptr };

		//Enemy* m_enemy{ nullptr };
};