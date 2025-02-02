#include "Gameplay/Enemy.h"
#include "Gameplay/Tower.h"
#include "Gameplay/Projectile.h"

Tower::~Tower()
{
    deleteProjectiles();
}

void Tower::update(float deltaMilliSeconds)
{

    lastAttackTime += deltaMilliSeconds;

	Enemy* closestEnemy = findTarget();

    if (closestEnemy != nullptr)
    {
        if (closestEnemy != nullptr)
        {
            if (lastAttackTime >= timeBetweenAttacks)
            {
                lastAttackTime = 0.0f;
                shoot(closestEnemy);
            }
        }
    }

    for (auto& projectile : m_projectileArray)
    {
        if (projectile.second)
        {
            projectile.second->update(deltaMilliSeconds);

            if (projectile.second->getImpact())
            {
                delete projectile.second;
                projectile.second = { nullptr };
                closestEnemy->setKilled();
            }

        }
    }
}

void Tower::render(sf::RenderWindow& window)
{
    for (auto& projectile : m_projectileArray)
    {
        if (projectile.second)
        {
            projectile.second->render(window);
        }
    }
}

void Tower::setEnemyArray(std::unordered_map<int, Enemy*>* enemyArray)
{
	m_enemyArray = enemyArray;
}

int Tower::getNextAviableID()
{
    id++;

    return id;
}

Enemy* Tower::findTarget()
{
    if (!m_enemyArray || m_enemyArray->empty()) {
        return nullptr;
    }

    Enemy* closestEnemy = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (auto& [id, enemy] : *m_enemyArray)
    {
        if (!enemy) continue;

        float distance = std::hypot(
            enemy->getPosition().x - getPosition().x,
            enemy->getPosition().y - getPosition().y
        );

        if (distance < minDistance)
        {
            minDistance = distance;
            closestEnemy = enemy;
        }
    }

    return closestEnemy;
}

void Tower::createProjectile(Enemy* target)
{
    id = getNextAviableID();

    Projectile* projectile = new Projectile();
    projectile->init();
    projectile->setTarget(target);
    projectile->setPosition(m_position);

    m_projectileArray[id] = projectile;
}

void Tower::shoot(Enemy* target)
{
    createProjectile(target);
}

void Tower::deleteProjectiles(bool end)
{  
    for (auto& projectile : m_projectileArray)
    {
        delete projectile.second;
    }
    m_projectileArray.clear();
    return;    
}

