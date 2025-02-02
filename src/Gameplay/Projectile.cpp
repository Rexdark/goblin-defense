#include <SFML/Graphics.hpp>

#include "Gameplay/Enemy.h"
#include "Gameplay/Projectile.h"


void Projectile::init()
{
	return;
}

void Projectile::setTarget(Enemy* target)
{
    if (target == nullptr)
    {
        printf("Target null!");
    }

	m_target = target;
}

void Projectile::update(float deltaMilliseconds)
{
    moveProjectile(deltaMilliseconds);

    if (impacted())
    {
        impact = true;
    }

    if (m_target == nullptr || m_target->getKilled())
    {
        renderProjectile = false;
    }
}

void Projectile::render(sf::RenderWindow& window)
{
    if (renderProjectile)
    {
        sf::CircleShape circle(projectileRadious / 2);
        circle.setFillColor(sf::Color::Black);
        circle.setPosition(m_position);
        window.draw(circle);
    }
}

bool Projectile::getImpact() const
{
	return impact;
}

void Projectile::moveProjectile(float deltaMilliseconds)
{
    if (m_target!= nullptr) 
    {
        m_targetLastPosition = m_target->getPosition();
    }

    m_direction = m_targetLastPosition - m_position;

    float length = std::sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
    if (length > 0)
    {
        m_direction /= length;
    }

    sf::Vector2f movement;
    movement.x = m_direction.x * m_speed.x * (deltaMilliseconds / 1000.0f);
    movement.y = m_direction.y * m_speed.y * (deltaMilliseconds / 1000.0f);
    
    m_position += movement;

    setPosition(m_position);
}

bool Projectile::impacted()
{
    if (m_target != nullptr)
    {
        m_targetLastPosition = m_target->getPosition();
    }

    if (std::abs(m_position.x - m_targetLastPosition.x) < 2.f &&
        std::abs(m_position.y - m_targetLastPosition.y) < 2.f)
    {
        return true;
    }

    return false;
}
