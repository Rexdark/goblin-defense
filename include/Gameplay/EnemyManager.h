#pragma once

#include <string>
#include <unordered_map>

#include <Gameplay/Enemy.h>

namespace sf
{
	class RenderWindow;
}

class Level;

class EnemyManager
{
	public:

		static EnemyManager* getInstance();

		void init(Level* leve);

		~EnemyManager();

		void spawnEnemies(int numberOfEnemies, std::string enemyType, std::vector<Level::Tile> spawnTileArray);
		Enemy* getEnemy(const int id);
		void deleteEnemy(const int id);

		int getGoblinsEscaped();

		bool checkEnemyArrayEmpty();

		void update(uint32_t deltaMilliseconds);
		void render(sf::RenderWindow& window);

	private:

		int goblinsEscaped = 0;

		static EnemyManager* em_instance;
		Level* m_level{ nullptr };

		int m_nextID = 0;

		std::unordered_map<int, Enemy*> enemyArray;

		std::vector<int> enemiesToDelete;

		void newEnemy(std::string enemyType, float positionWidth, float positionHeight);

		int getNextAvaiableID();
		Level::Tile selectRandomTile(std::vector<Level::Tile> spawnTileArray);

		Enemy* getEnemyClassByType(const std::string enemyType);
		std::string getEnemyConfigPathbyType(std::string enemyType);
		
		Enemy::EnemyDescriptor* loadFromConfig(const std::string configPath);

		void clear();
};