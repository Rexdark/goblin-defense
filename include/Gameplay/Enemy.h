#pragma once

#include <string>
#include <vector>

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

class MapLayer;

class Enemy : public GameObject
{
	public:

		struct EnemyDescriptor
		{
			sf::Vector2f position;
			sf::Vector2f speed{ .0f, .0f };
			sf::Sprite* sprite{ nullptr };
			int Width{ 0 };
			int Height{ 0 };
			int frameCount{ 0 };
			float frameTime{ 50.0f };
		};

		bool init( const EnemyDescriptor& enemyDescriptor, MapLayer* mapLayer);

		~Enemy() override = default;

		sf::FloatRect getBounds() const { return m_sprite->getGlobalBounds(); };

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

	protected:

		struct Node
		{
			sf::Vector2f position;
			float gCost{ 0.0f };  // Cost from start to current node
			float hCost{ 0.0f };  // Heuristic cost to the goal
			float fCost() const { return gCost + hCost; }  // Total cost
			Node* parent{ nullptr };

			bool operator>(const Node& other) const { return fCost() > other.fCost(); }  // For priority queue
		};

		sf::Vector2f searchNextTileBasedOnPath(sf::Vector2f destinationTile);

		const std::vector<uint32_t> m_impassableTiles = { 55, 56, 57, 67, 68, 69, 79, 80, 81, 91, 92, 103, 104, 158, 153, 164, 165, 176, 177, 188,  189 };

		MapLayer* m_mapLayer{ nullptr };

		sf::Sprite* m_sprite{nullptr};
		float m_tileWidth{ .0f };
		float m_tileHeight{ .0f };

		int m_frameCount{ 1 };
		int m_currentFrame{ 0 };
		float m_frameTime{ 100.f };
		float m_elapsedTime{ 0.f };

		sf::Vector2f m_speed{ .0f, .0f };
		sf::Vector2f m_destination{ .0f, .0f };
		sf::Vector2f m_nextTileCoordinates{ .0f, .0f };
		sf::Vector2f m_direction{ .0f, .0f };

		void moveEnemy(float deltaMilliseconds);
};