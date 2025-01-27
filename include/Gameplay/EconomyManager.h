#pragma once

class EconomyManager
{
	public:

		EconomyManager();

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

	private: //Configure and add resource mine levels if there's enough time. They are not necessary for the demo.

		struct resources 
		{
			int requisitionPoints = 0;
			int food = 0;
			int wood = 0;
			int stone = 0;
		};

		resources m_currentResources = {};
		bool m_currentResourcesInitialized = false;

		int m_currentYear = 0;

		enum seasons {Winter, Spring, Summer, Autumn};

		seasons m_currentSeason = {};

		resources startingResourcesAllocator(int requisitionPoints, int food, int wood, int stone);

		std::string seasonToString(seasons season) const;
};