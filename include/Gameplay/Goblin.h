#pragma once
#include <string>

#include <Gameplay/Enemy.h>


class Goblin : public Enemy
{
	public:

		struct GoblinDescriptor : EnemyDescriptor
		{
		};

		bool init(const GoblinDescriptor& enemyDescriptor, MapLayer* mapLayer);

		void update(float deltaMilliseconds) override;

	private:
};