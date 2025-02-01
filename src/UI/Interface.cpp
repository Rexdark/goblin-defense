#include <vector>
#include <thread>

#include <SFML/Graphics.hpp>

#include <Core/Level.h>
#include <Gameplay/EconomyManager.h>
#include <Gameplay/EnemyManager.h>
#include "UI/Interface.h"
#include <Utils/AssetManager.h>
#include <Utils/Constants.h>

void Interface::interfaceInit(float width, float height, AssetManager* assetManager, EconomyManager* economyManager, EnemyManager* enemyManager, Level* level)
{
    m_economyManager = economyManager;
    m_level = level;
    m_enemyManager = enemyManager;

    if (!m_font.loadFromFile(FONT))
    {
        printf("Game font could not be loaded at %s.\n", FONT);
    }

    //Resources
    m_resourceBar.setSize(sf::Vector2f(width, height/15));
    m_resourceBar.setFillColor(sf::Color::Black);
    m_resourceBar.setPosition(0.f, 0.f);

    m_interfaceItems.clear();
    m_interfaceItems.push_back(m_requisitionPointsText);
    m_interfaceItems.push_back(m_foodText);
    m_interfaceItems.push_back(m_woodText);
    m_interfaceItems.push_back(m_stoneText);
    m_interfaceItems.push_back(m_seasonYearText);

    for (size_t i = 0; i < m_interfaceItems.size(); ++i) 
    {
        m_interfaceItems[i].setFont(m_font);
        m_interfaceItems[i].setString(getResourceTextString(i, economyManager));
        m_interfaceItems[i].setCharacterSize(8);
        m_interfaceItems[i].setFillColor(sf::Color::White);
        m_interfaceItems[i].setPosition(getResourceTextPosition(i, width, 5));
    }

    //Next turn button
    m_nextTurnButton.setSize(sf::Vector2f(150.f, 50.f));
    m_nextTurnButton.setFillColor(sf::Color::Black);
    m_nextTurnButton.setPosition(width-150.f, height-50.f);

    m_nextTurnText.setFont(m_font);
    m_nextTurnText.setString("Next Season");
    m_nextTurnText.setCharacterSize(10);
    m_nextTurnText.setFillColor(sf::Color::White);
    m_nextTurnText.setPosition(width - 130.f, height - 30.f);

    m_buttons.push_back(&m_nextTurnText);

    //Pause Menu
    m_menuBackground.setSize(sf::Vector2f(width/2, height / 2));
    m_menuBackground.setFillColor(sf::Color::Black);
    m_menuBackground.setPosition(width / 4, height / 4);

    m_pauseMenuItems.clear();
    m_pauseMenuItems.push_back(resumeText);
    m_pauseMenuItems.push_back(saveGameText);
    m_pauseMenuItems.push_back(exitGameText);

    for (size_t i = 0; i < m_pauseMenuItems.size(); ++i) 
    {
        m_pauseMenuItems[i].setFont(m_font);
        m_pauseMenuItems[i].setString(getPauseMenuTextString(i));
        m_pauseMenuItems[i].setCharacterSize(10);
        m_pauseMenuItems[i].setFillColor(sf::Color::White);
        m_pauseMenuItems[i].setPosition(getPauseMenuTextPosition(i, width, height));

        m_buttons.push_back(&m_pauseMenuItems[i]);
    }
}

bool Interface::update(sf::RenderWindow* window)
{
    buildingLogic(window);

    if (inCombat)
    {
        if (m_enemyManager->checkEnemyArrayEmpty())
        {
            inCombat = false;
        }
    }

    bool exitMenu = interfaceLogic(window); //Go back to main menu if true

    return exitMenu;
}

void Interface::draw(sf::RenderWindow& window)
{
    //Resources
    window.draw(m_resourceBar);
    for (size_t i = 0; i < m_interfaceItems.size(); ++i) 
    {
        window.draw(m_interfaceItems[i]);
    }

    //Next Turn Button
    if (!inCombat)
    {
        window.draw(m_nextTurnButton);
        window.draw(m_nextTurnText);
    }

    //Pause Menu
    if (pauseMenu)
    {
        window.draw(m_menuBackground);
        for (size_t i = 0; i < m_pauseMenuItems.size(); ++i)
        {
            window.draw(m_pauseMenuItems[i]);
        }
    }
}

std::string Interface::getResourceTextString(int index, const EconomyManager* economyManager) const
{
    if (index == 0)
    {
        return "Requisition: " + std::to_string(economyManager->getRequisitionPoints());
    }
    else if (index == 1)
    {
        return "Food: " + std::to_string(economyManager->getFood());
    }
    else if (index == 2)
    {
        return "Wood: " + std::to_string(economyManager->getWood());
    }
    else if (index == 3)
    {
        return "Stone: " + std::to_string(economyManager->getStone());
    }
    else if (index == 4)
    {
        return economyManager->getCurrentSeason() + ", " + std::to_string(economyManager->getCurrentYear()) + " CV"; //Again dnd fan
    }
    else
    {
        printf("Unknown UI Element!\n");
        return "";
    }
}

sf::Vector2f Interface::getResourceTextPosition(int index, int windowWidth, int height) const
{
    if (index == 0) //Requisiton Points
    {
        return sf::Vector2f(25.f, height);
    }
    else if (index == 1) //Food
    {
        return sf::Vector2f(200.f, height);
    }
    else if (index == 2) //Wood
    {
        return sf::Vector2f(300.f, height);
    }
    else if (index == 3) //Stone
    {
        return sf::Vector2f(400.f, height);
    }
    else if (index == 4) //Date
    {
        return sf::Vector2f(windowWidth - 120.f, height);
    }
    else
    {
        printf("Unknown UI Element!\n");
        return sf::Vector2f(0, 0);
    }
}

std::string Interface::getPauseMenuTextString(int index) const
{
    if (index == 0)
    {
        return "Resume Game";
    }
    else if (index == 1)
    {
        return "Save Game";
    }
    else if (index == 2)
    {
        return "Exit Game";
    }
    else
    {
        printf("Unknown UI Element!\n");
        return "";
    }
}

sf::Vector2f Interface::getPauseMenuTextPosition(int index, int width, int height) const
{
    if (index == 0) //Resume Game
    {
        return sf::Vector2f(width / 8 * 3.25, height / 8 * 2.75);
    }
    else if (index == 1) //Save Game
    {
        return sf::Vector2f(width / 8 * 3.25, height / 8 * 3.75);
    }
    else if (index == 2) //Exit Game
    {
        return sf::Vector2f(width / 8 * 3.25, height / 8 * 4.75);
    }
    else
    {
        printf("Unknown UI Element!\n");
        return sf::Vector2f(0, 0);
    }
}

bool Interface::interfaceLogic(sf::RenderWindow* window)
{
    sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

    bool buttonFound = false;
    
    int selectedItemIndex = -1;

    for (int i = 0; i < m_buttons.size(); ++i)
    {
        sf::FloatRect bounds = m_buttons[i]->getGlobalBounds();

        if (bounds.contains(mousePos))
        {
            if (selectedItemIndex != -1)
            {
                m_buttons[selectedItemIndex]->setFillColor(sf::Color::White);
            }

            selectedItemIndex = i;
            m_buttons[selectedItemIndex]->setFillColor(sf::Color::Red);

            buttonFound = true;
            break;
        }
    }

    if (!buttonFound)
    {
        for(int i = 0; i < m_buttons.size(); ++i)
        {
            m_buttons[i]->setFillColor(sf::Color::White);
        }
        selectedItemIndex = -1;
        mouseOverMenu = false;
    }
    else
    {
        mouseOverMenu = true;
        m_level->setBuildMode(false);
    }

    //printf("Selected item index %d.\n", selectedItemIndex);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttonFound && !inCombat)
    {
        return executeLogicAction(selectedItemIndex);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        if (pauseMenu)
        {
            pauseMenu = false;
        }
        else
        {
            pauseMenu = true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(250)); //Avoids blinking interface.
    }

    return false;
}

bool Interface::executeLogicAction(int index)
{
    if (index == 0) //Next Turn
    {
        nextTurn();
    }
    else if (index == 1 && pauseMenu) //Resume Game
    {
        pauseMenu = false;
    }
    else if (index == 2 && pauseMenu) //Save game
    {
        m_level->saveToFile();
        pauseMenu = false;
    }
    else if (index == 3 && pauseMenu) //Exit Game
    {
        return true;
    }

    return false;
}

void Interface::nextTurn()
{
    m_economyManager->nextTurn();
    inCombat = true;
    m_enemyManager->EnemyManager::getInstance()->spawnEnemies(5, "Goblin", m_level->getEnemySpawnTiles());
    updateTexts();
}

void Interface::updateTexts()
{
    for (size_t i = 0; i < m_interfaceItems.size(); ++i) {
        m_interfaceItems[i].setString(getResourceTextString(i, m_economyManager));
    }
}

void Interface::buildingLogic(sf::RenderWindow* window)
{
    if (!mouseOverMenu && checkBuildingMode() && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        m_level->renderBuildinginBuildMode(window, true);
        std::this_thread::sleep_for(std::chrono::milliseconds(250)); //Avoids blinking interface.
        m_economyManager->deduceResources(m_economyManager->towerCost); //Should be changed by m_currentBuilding when more buildings are avaiable
        updateTexts();
    }
}

bool Interface::checkBuildingMode()
{
    if (inCombat)
    {
        return false;
    }

    if (m_economyManager->checkBuildingCosts(m_economyManager->towerCost)) //Should be changed by m_currentBuilding when more buildings are avaiable
    {
        m_level->setBuildMode(true);
    }
    else
    {
        m_level->setBuildMode(false);
    }

    return m_level->getBuildMode();
}
