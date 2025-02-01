#include <Core/Level.h>
#include <Core/World.h>
#include <Gameplay/EconomyManager.h>
#include <Gameplay/EnemyManager.h>
#include <UI/Interface.h>

bool World::load(bool newGame, float width, float height, AssetManager* assetManager)
{

	m_level = new Level();
	m_level->init(newGame);

	m_economyManager = new EconomyManager();

	EnemyManager* enemymanager = new EnemyManager();
	m_enemyManager = enemymanager->getInstance();

	m_enemyManager->getInstance()->init(m_level);

	m_interface = new Interface();	
	m_interface->interfaceInit(width, height, assetManager, m_economyManager, m_enemyManager, m_level);

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

bool World::update(uint32_t deltaMilliseconds, sf::RenderWindow* window)
{
	m_level->update(window);
	m_enemyManager->update(deltaMilliseconds);

	bool mainMenu = m_interface->update(window);

	return mainMenu;
}

void World::render(sf::RenderWindow& window)
{
	m_level->render(window);
	m_enemyManager->render(window);//Should render between later 1 and 2 of level.


	m_interface->draw(window);
}