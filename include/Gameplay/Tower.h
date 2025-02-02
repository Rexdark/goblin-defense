#pragma once

#include <Gameplay/GameObject.h>
#include <Gameplay/Projectile.h>
#include <unordered_map>

class Enemy;
class Level;

class Tower : public GameObject
{
	public:

		~Tower();

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		void setEnemyArray(std::unordered_map<int, Enemy*>* enemyArray);

	private:

		int id = 0;

		float timeBetweenAttacks = 1000.0f;

		float lastAttackTime = .0f;

		std::unordered_map<int, Projectile*> m_projectileArray;

		std::unordered_map<int, Enemy*>* m_enemyArray{ nullptr };

		int getNextAviableID();

		Enemy* findTarget();
		void createProjectile(Enemy* Target);

		void shoot(Enemy* target);
		void deleteProjectiles(bool end = false);
};