#include <cassert>
#include <cstdio>
#include <string>
#include <vector>

#include "Core/Level.h"
#include "Gameplay/EconomyManager.h"
#include "Gameplay/EnemyManager.h"

EconomyManager::EconomyManager()
{
	int startingResources[]{ 1000, 500, 500, 500 }; //Configure here starting resources. Not loaded from disk to avoid cheating. Might be customizabe in the future with difficulty option settings.

	m_currentResources = startingResourcesAllocator(startingResources[0], startingResources[1], startingResources[2], startingResources[3]);

	m_currentYear = 1356; //Fan of dnd I guess? (Had to pick a random number since there's no lore.)

	m_currentSeason = { Winter }; //Maybe let the player configure starting season in the future. Currently seasons have no effect besides allow to sense passage of time.
}

EconomyManager::resources EconomyManager::startingResourcesAllocator(int requisitionPoints, int food, int wood, int stone)
{
	if(m_currentResourcesInitialized)
	{
		printf("Current resources are already initialized!!");
		return {};
	}

	resources startingResources = {};

	startingResources.requisitionPoints = requisitionPoints;
	startingResources.food = food;
	startingResources.wood = wood;
	startingResources.stone = stone;

	m_currentResourcesInitialized = true;

	return startingResources;
}

void EconomyManager::deduceResources(resources cost)
{
	m_currentResources.requisitionPoints -= cost.requisitionPoints;
	m_currentResources.food -= cost.food;
	m_currentResources.wood -= cost.wood;
	m_currentResources.stone -= cost.stone;
}

void EconomyManager::nextTurn()
{
	int default_ammount = 500; //This should be a method that calculates the correct ammount when resource mines are implemented.

	m_currentResources.requisitionPoints += default_ammount;

	m_currentResources.food += default_ammount;

	m_currentResources.wood += default_ammount;

	m_currentResources.stone += default_ammount;

	switch (m_currentSeason)
	{
		case Winter:

			m_currentSeason = Spring;
			break;

		case Spring:

			m_currentSeason = Summer;
			break;

		case Summer:

			m_currentSeason = Autumn;
			break;

		case Autumn:

			m_currentSeason = Winter;
			m_currentYear += 1;
			break;

		default:

			printf("Unregonzided current season state! Going back to default!\n");
			m_currentSeason = Winter;
			break;
	}
}

int EconomyManager::getRequisitionPoints() const
{
	return m_currentResourcesInitialized ? m_currentResources.requisitionPoints : 0;
}

void EconomyManager::setRequisitionPoints(int value)
{
	if (m_currentResourcesInitialized)
		m_currentResources.requisitionPoints = value;
}

int EconomyManager::getFood() const
{
	return m_currentResourcesInitialized ? m_currentResources.food : 0;
}

void EconomyManager::setFood(int value)
{
	if (m_currentResourcesInitialized)
		m_currentResources.food = value;
}

int EconomyManager::getWood() const
{
	return m_currentResourcesInitialized ? m_currentResources.wood : 0;
}

void EconomyManager::setWood(int value)
{
	if (m_currentResourcesInitialized)
		m_currentResources.wood = value;
}

int EconomyManager::getStone() const
{
	return m_currentResourcesInitialized ? m_currentResources.stone : 0;
}

void EconomyManager::setStone(int value)
{
	if (m_currentResourcesInitialized)
		m_currentResources.stone = value;
}

int EconomyManager::getCurrentYear() const
{
	return m_currentYear;
}

void EconomyManager::setYear(int value)
{
	m_currentYear = value;
}

std::string EconomyManager::getCurrentSeason() const
{
	return seasonToString(m_currentSeason);
}

void EconomyManager::setCurrentSeason(int season)
{
	if (season >= Winter && season <= Autumn)
		m_currentSeason = static_cast<seasons>(season);
}

std::string EconomyManager::seasonToString(seasons season) const
{
	switch (season) 
	{
		case Winter: 
			
			return "Winter";
			break;

		case Spring:
			
			return "Spring";
			break;

		case Summer: 
			
			return "Summer";
			break;

		case Autumn: 
			
			return "Autumn";
			break;

		default: 
			
			return "Unknown Value!";
			break;
	}
}

bool EconomyManager::checkBuildingCosts(resources selectedBuilding)
{
	if (m_currentResources.requisitionPoints >= selectedBuilding.requisitionPoints &&
		m_currentResources.food >= selectedBuilding.food &&
		m_currentResources.wood >= selectedBuilding.wood &&
		m_currentResources.stone >= selectedBuilding.stone)
	{
		return true;
	}

	return false;
}
