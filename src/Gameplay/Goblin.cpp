#include <Gameplay/Goblin.h>

bool Goblin::init(const GoblinDescriptor& goblinDescriptor, MapLayer* mapLayer)
{	
	return Enemy::init(goblinDescriptor, mapLayer);
}

void Goblin::update(float deltaMilliseconds)
{
	if (m_position != m_destination)
	{
		if (std::abs(m_nextTileCoordinates.x) < 0.001f || std::abs(m_nextTileCoordinates.y) > 0.001f)
		{
			m_nextTileCoordinates = searchNextTileBasedOnPath(m_destination);
		}

		if (m_position != m_nextTileCoordinates)
		{
			moveEnemy(deltaMilliseconds);
		}
		else
		{
			m_nextTileCoordinates = searchNextTileBasedOnPath(m_destination);
		}
	}

	

	Enemy::update(deltaMilliseconds);
}
