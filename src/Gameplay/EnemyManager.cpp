#include <fstream>
#include <filesystem>
#include <random>

#include "Core/Level.h"
#include "Gameplay/Enemy.h"
#include "Gameplay/EnemyManager.h"
#include "Gameplay/Goblin.h"
#include <Utils/Constants.h>
#include <Utils/Tools.h>

EnemyManager* EnemyManager::em_instance = nullptr;

EnemyManager* EnemyManager::getInstance()
{
	if (em_instance == nullptr)
	{
		em_instance = new EnemyManager();
	}
	return em_instance;
}

void EnemyManager::init(Level* level)
{
	m_level = level;
}

EnemyManager::~EnemyManager()
{
	clear();
}

int EnemyManager::newEnemy(std::string enemyType, float positionWidth, float positionHeight)
{
	int id = getNextAvaiableID();

	Enemy* enemy = getEnemyClassByType(enemyType);
	
	if (enemy == nullptr)
	{
		printf("Invalid enemy class %s!\n", enemyType.c_str());
		return 0;
	}

	//printf("Creating enemy: %s\n", enemyType.c_str());

	Enemy::EnemyDescriptor* enemyDescriptor = loadFromConfig(getEnemyConfigPathbyType(enemyType));

	if (enemyDescriptor == nullptr)
	{
		printf("Could not get a descriptor!!\n");
		return 0;
	}

	enemyDescriptor->position = { positionWidth, positionHeight };
	enemy->init(*enemyDescriptor, m_level->getPathSearchMapLayer());
	enemy->setPosition(enemyDescriptor->position);

	//printf("Creating enemy %s at position %f,%f\n", enemyType.c_str(), enemyDescriptor->position.x, enemyDescriptor->position.y);

	enemyArray[id] = enemy;

	return id;
}

void EnemyManager::spawnEnemies(int numberOfEnemies, std::string enemyType, std::vector<Level::Tile> spawnTileArray)
{
	for (int i = 0; i < numberOfEnemies; i++)
	{
		Level::Tile spawnTile = selectRandomTile(spawnTileArray);

		sf::Vector2i SpawnCoordinates = m_level->getTileCoordinates(spawnTile.col, spawnTile.row);

		newEnemy(enemyType, SpawnCoordinates.x, SpawnCoordinates.y);
	}

}

Enemy* EnemyManager::getEnemy(int id)
{
	auto enemy = enemyArray.find(id);

	if (enemy != enemyArray.end())
	{
		return enemy->second;
	}
	else
	{
		return { nullptr };
	}
}

void EnemyManager::deleteEnemy(int id)
{
	auto enemy = enemyArray.find(id);

	if (enemy != enemyArray.end())
	{
		delete enemy->second;
		enemyArray.erase(enemy);
	}
}

bool EnemyManager::checkEnemyArrayEmpty()
{
	if (enemyArray.empty())
	{
		return true;
	}

	return false;
}

void EnemyManager::update(uint32_t deltaMilliseconds)
{
	for (auto& it : enemyArray)
	{
		it.second->update(deltaMilliseconds);
	}
}

void EnemyManager::render(sf::RenderWindow& window)
{
	for (auto& it : enemyArray)
	{
		it.second->render(window);
	}
}

int EnemyManager::getNextAvaiableID()
{
	m_nextID++;

	return m_nextID;
}

Level::Tile EnemyManager::selectRandomTile(std::vector<Level::Tile> spawnTileArray)
{
	Level::Tile tile = {};

	if (spawnTileArray.empty())
	{
		printf("Enemy tile spawn pool is empty!\n");
		return tile;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, spawnTileArray.size() - 1);
	int randomIndex = dis(gen);

	tile = spawnTileArray[randomIndex];

	printf("Selected spawn tile [%d][%d]\n", tile.row, tile.col);

	return tile;
}

Enemy* EnemyManager::getEnemyClassByType(std::string enemyType)
{
	Enemy* enemy{ nullptr };

	if (enemyType == "Goblin")
	{
		enemy = new Goblin();
	}

	return enemy;
}

std::string EnemyManager::getEnemyConfigPathbyType(std::string enemyType)
{
	if (enemyType == "Goblin")
	{
		return ENEMY_GOBLIN_PATH;
	}

	return ENEMY_GOBLIN_PATH; //Default Enemy
}

Enemy::EnemyDescriptor* EnemyManager::loadFromConfig(std::string configPath)
{
	Enemy::EnemyDescriptor* enemyDescriptor = new Enemy::EnemyDescriptor();

	if (!fileExists(configPath))
	{
		printf("Config file '%s' does not exist! Creating config file with default options!\n", configPath.c_str());
		return enemyDescriptor;
	}

	std::ifstream configFile(configPath);

	if (!configFile.is_open())
	{
		printf("Failed to open the config file!\n");
		return enemyDescriptor;
	}

	std::string line;

	while (std::getline(configFile, line))
	{
		if (line.find("Texture=") == 0)
		{
			std::string texturePath = line.substr(8);
			sf::Sprite* enemySprite = AssetManager::getInstance()->getSprite(texturePath);
			enemySprite->setScale(0.25f, 0.25f);
			enemyDescriptor->sprite = enemySprite;
		}
		else if (line.find("TileWidth=") == 0)
		{
			enemyDescriptor->Width = std::stoi(line.substr(10));
		}
		else if (line.find("TileHeight=") == 0)
		{
			enemyDescriptor->Height = std::stoi(line.substr(11));
		}
		else if (line.find("FrameCount=") == 0)
		{
			enemyDescriptor->frameCount = std::stoi(line.substr(11));
		}
		else if (line.find("FrameTime=") == 0)
		{
			enemyDescriptor->frameTime = std::stof(line.substr(10));
		}
	}

	configFile.close();

	return enemyDescriptor;
}

void EnemyManager::clear()
{
	for (auto& it : enemyArray)
	{
		delete it.second;
	}
	enemyArray.clear();
}
