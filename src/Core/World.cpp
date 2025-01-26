#include <Core/Level.h>
#include <Core/World.h>

World::~World()
{
	delete m_level;
	//delete m_layerZero;
	//delete m_layerOne;
	//delete m_layerTwo;
}

bool World::load()
{
	//printf("Creating level.\n");

	m_level = new Level();
	m_level->init(); //Set argument to true to clear current map.

	if (m_level)
	{
		//printf("Level created.\n");

		return true;
	}
	else
	{
		//printf("Level couldn't be created correctly.\n");

		return false;
	}
}

void World::unload()
{
	delete m_level;
}

//void World::update(uint32_t deltaMilliseconds)
//{
//	// To-Do: update level
//	m_layerZero->update(sf::milliseconds(deltaMilliseconds));
//
//	// Update actors
//	m_enemy->update(deltaMilliseconds);
//
//	// Check for collisions (We could do it in a function here or have a collision manager if it gets complex)
//	const auto& collisionShapes = m_collisionLayer->getShapes();
//	for (const auto* shape : collisionShapes)
//	{
//		if (shape->getGlobalBounds().intersects(m_enemy->getBounds()))
//		{
//#if DEBUG_MODE
//			printf("Collision is detected");
//#endif
//		}
//	}
//}

void World::update(uint32_t deltaMilliseconds)
{

}

void World::render(sf::RenderWindow& window)
{
	/*window.draw(*m_layerZero);
	window.draw(*m_layerOne);
	window.draw(*m_layerTwo);
	window.draw(*m_collisionLayer);
	m_enemy->render(window);*/

	m_level->render(window);
}