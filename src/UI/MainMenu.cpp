#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

#include <Core/Game.h>
#include <UI/MainMenu.h>
#include <Utils/AssetManager.h>
#include <Utils/Constants.h>

MainMenu::MainMenu(float width, float height, AssetManager* assetManager)
{
    m_background = assetManager->getTexture("backgrounds/main_menu.png");

    m_backgroundCanvas.setPosition(0, 0);
    m_backgroundCanvas.setSize(sf::Vector2(width, height));
    m_backgroundCanvas.setTexture(m_background);

    if (!m_font.loadFromFile(FONT)) 
    {
        printf("Game font could not be loaded at %s.\n", FONT);
    }

    std::vector<std::string> options = { "New Game", "Load Game", "Exit" };

    m_gameTitle.setFont(m_font);
    m_gameTitle.setString(GAME_TITLE);
    m_gameTitle.setCharacterSize(36);
    m_gameTitle.setFillColor(sf::Color::Black);
    m_gameTitle.setPosition(width / 2 - m_gameTitle.getGlobalBounds().width / 2, height/10);

    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text;
        text.setFont(m_font);
        text.setString(options[i]);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        text.setPosition(width / 2 - text.getGlobalBounds().width / 2, height / (options.size() + 1) * (i + 1.25));
        m_menuItems.push_back(text);
    }

    m_selectedItemIndex = -1;
}

void MainMenu::draw(sf::RenderWindow& window) 
{
    window.draw(m_backgroundCanvas);
    window.draw(m_gameTitle);

    for (const auto& item : m_menuItems) {
        window.draw(item);
    }
}

int MainMenu::menuLogic(sf::RenderWindow* window)
{
    sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

    bool buttonFound = false;

    for (size_t i = 0; i < m_menuItems.size(); ++i) 
    {
        sf::FloatRect bounds = m_menuItems[i].getGlobalBounds();

        if (bounds.contains(mousePos)) 
        {
            if (m_selectedItemIndex != -1)
            {
                m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::Black);
            }
            m_selectedItemIndex = i; 
            m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::Red);

            buttonFound = true;
            break;
        }
    }

    if (!buttonFound)
    {
        if (m_selectedItemIndex != -1)
        {
            m_menuItems[m_selectedItemIndex].setFillColor(sf::Color::Black);
        }
        m_selectedItemIndex = -1;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        return m_selectedItemIndex;
    }

    return -1;
}