#pragma once

#include <Gameplay/GameObject.h>

class Enemy;

class Projectile : public GameObject
{
	public:


		void init();

		void setTarget(Enemy* target);

		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

		bool getImpact() const;

	private:

		bool impact = false;
		bool renderProjectile = true;
		
		float projectileRadious{ 2.0f };

		sf::Vector2f m_direction{ .0f, .0f };
		sf::Vector2f m_speed{ 256.f, 256.f };

		sf::Vector2f m_targetLastPosition{};

		Enemy* m_target{ nullptr };

		void moveProjectile(float deltaMilliseconds);
		bool impacted();
};