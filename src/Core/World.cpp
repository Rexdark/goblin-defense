#include <Core/Level.h>
#include <Core/World.h>
#include <Gameplay/EconomyManager.h>
#include <UI/Interface.h>

bool World::load(bool newGame, float width, float height, AssetManager* assetManager)
{

	m_level = new Level();
	m_level->init(newGame);

	m_economyManager = new EconomyManager();

	m_interface = new Interface();	
	m_interface->interfaceInit(width, height, assetManager, m_economyManager, m_level);

	if (m_level)
	{
		return true;
	}
	else
	{
		printf("There was an error loading the map!\n");
		return false;
	}
}

void World::unload()
{
	if (m_level)
	{
		delete m_level;
		m_level = { nullptr };
	}
	if (m_economyManager)
	{
		delete m_economyManager;
		m_economyManager = { nullptr };
	}
	if (m_interface)
	{
		delete m_interface;
		m_interface = { nullptr };
	}
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

bool World::update(uint32_t deltaMilliseconds, sf::RenderWindow* window)
{
	return m_interface->update(window);
}

void World::render(sf::RenderWindow& window)
{
	/*window.draw(*m_layerZero);
	window.draw(*m_layerOne);
	window.draw(*m_layerTwo);
	window.draw(*m_collisionLayer);
	m_enemy->render(window);*/

	m_level->render(window);
	m_interface->draw(window);

}