#pragma once

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

class MainMenu 
{
    public:
        
        MainMenu(float width, float height, AssetManager* assetManager);

        int menuLogic(sf::RenderWindow* window);

        void draw(sf::RenderWindow& window);

    private:

        sf::Texture* m_background{ nullptr };

        sf::RectangleShape m_backgroundCanvas;

        sf::Text m_gameTitle;

        std::vector<sf::Text> m_menuItems;

        sf::Font m_font;

        int m_selectedItemIndex;
};