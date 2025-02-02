#include <Core/Level.h>
#include <Gameplay/Goblin.h>

bool Goblin::init(const GoblinDescriptor& goblinDescriptor, std::vector<std::vector<uint32_t>> pathMapVector, Level* level)
{	
	return Enemy::init(goblinDescriptor, pathMapVector, level);
}

void Goblin::update(float deltaMilliseconds)
{

	if ((std::abs(m_destination.x - m_position.x > 0.1f) || std::abs(m_destination.y - m_position.y > 0.1f)))
	{

		if (m_position != m_nextTileCoordinates)
		{
			moveEnemy(deltaMilliseconds);
		}

		if ((std::abs(m_nextTileCoordinates.x < 0.001f) || std::abs(m_nextTileCoordinates.y < 0.001f)) ||
			(std::abs(m_nextTileCoordinates.x - m_position.x < 0.001f) || std::abs(m_nextTileCoordinates.y - m_position.y < 0.001f)))
		{

			sf::Vector2i nextTile = searchNextTileCoordinates();
			m_nextTileCoordinates = m_level->getTileCoordinates(nextTile.x, nextTile.y);
		}
	}

	Enemy::update(deltaMilliseconds);
}
