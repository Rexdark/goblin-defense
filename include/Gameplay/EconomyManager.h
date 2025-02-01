#pragma once

class Level;
class EnemyManager;

class EconomyManager
{
	public:

		EconomyManager();

		struct resources
		{
			int requisitionPoints = 0;
			int food = 0;
			int wood = 0;
			int stone = 0;
		};

		const resources towerCost = { 0, 100, 150, 200 };

		int getRequisitionPoints() const;
		void setRequisitionPoints(int value);

		int getFood() const;
		void setFood(int value);

		int getWood() const;
		void setWood(int value);

		int getStone() const;
		void setStone(int value);

		int getCurrentYear() const;
		void setYear(int value);

		std::string getCurrentSeason() const;
		void setCurrentSeason(int season);

		void nextTurn();

		bool checkBuildingCosts(resources selectedBuilding);
		void deduceResources(resources cost);

	private: //Configure and add resource mine levels if there's enough time. They are not necessary for the demo.

		resources m_currentResources = {};
		bool m_currentResourcesInitialized = false;

		int m_currentYear = 0;

		enum seasons {Winter, Spring, Summer, Autumn};

		seasons m_currentSeason = {};

		resources startingResourcesAllocator(int requisitionPoints, int food, int wood, int stone);

		std::string seasonToString(seasons season) const;
};