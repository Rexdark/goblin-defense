#pragma once

#include <string>
#include <unordered_set>

#include <Gameplay/GameObject.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Level;

class Enemy : public GameObject
{
	public:

		struct EnemyDescriptor
		{
			int id = 0;
			sf::Vector2f position;
			sf::Vector2f speed{ .0f, .0f };
			sf::Sprite* sprite{ nullptr };
			int Width{ 0 };
			int Height{ 0 };
			int frameCount{ 0 };
			float frameTime{ 50.0f };
		};

		bool init( const EnemyDescriptor& enemyDescriptor, std::vector<std::vector<uint32_t>> pathMapVector, Level* level);

		~Enemy() override = default;

		sf::FloatRect getBounds() const { return m_sprite->getGlobalBounds(); };
		void setDestination(sf::Vector2f destination);

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		bool getCompleteStatus() const;

		void setKilled();
		bool getKilled() const;

	protected:

		int id = 0;

		bool killed = false;

		bool atDestination = false;

		struct Node {
			sf::Vector2i position;
			float gCost = 0.0f;
			float hCost = 0.0f;
			float fCost() const { return gCost + hCost; }
			Node* parent = nullptr;

			bool operator>(const Node& other) const { return fCost() > other.fCost(); }
		};

		struct Vector2iHash {
			size_t operator()(const sf::Vector2i& v) const {
				return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
			}
		};

		Level* m_level{ nullptr };

		sf::Vector2i searchNextTileCoordinates();
		sf::Vector2i getCurrentTile();
		sf::Vector2i getDestinationTile();

		const std::unordered_set<uint32_t> m_impassableTiles = { 55, 56, 57, 67, 68, 69, 79, 80, 81, 91, 92, //Cliffs
																			58,59,60,70,71,72,82,83,84,94,95,//Forests
																									188,189, //Tower Bases			
																							145,146,147,148, //Village
																							157,158,159,160,
																							169,170,171,172,
																							181,182,183,184 };

		std::vector<std::vector<uint32_t>> m_pathMapVector = {};

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