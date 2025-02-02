#pragma once
#include <string>

#include <Gameplay/Enemy.h>


class Goblin : public Enemy
{
	public:

		struct GoblinDescriptor : EnemyDescriptor
		{
		};

		bool init(const GoblinDescriptor& enemyDescriptor, std::vector<std::vector<uint32_t>> pathMapVector, Level* level);

		void update(float deltaMilliseconds) override;

	private:
};