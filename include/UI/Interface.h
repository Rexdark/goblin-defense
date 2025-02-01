#pragma once

#include <SFML/Graphics.hpp>

class AssetManager;
class EconomyManager;
class Enemymanager;
class Level;

class Interface
{
    public:

        void interfaceInit(float width, float height, AssetManager* assetManager, EconomyManager* economyManager, EnemyManager* enemyManager, Level* level);

        bool update(sf::RenderWindow* window);

        void draw(sf::RenderWindow& window);

    private:

        bool pauseMenu = false;
        bool computingTurn = false;
        bool mouseOverMenu = false;
        bool inCombat = false;

        sf::Font m_font{};

        std::vector<sf::Text* > m_buttons{};

        //Resources
        sf::RectangleShape m_resourceBar{ sf::Vector2f(0, 0) };

        std::vector<sf::Text> m_interfaceItems{};
        sf::Text m_requisitionPointsText{};
        sf::Text m_foodText{};
        sf::Text m_woodText{};
        sf::Text m_stoneText{};
        sf::Text m_seasonYearText{};

        //Next Turn Button
        sf::RectangleShape m_nextTurnButton{ sf::Vector2f(0, 0) };
        sf::Text m_nextTurnText{};

        //Pause Menu
        sf::RectangleShape m_menuBackground{ sf::Vector2f(0, 0) };

        std::vector<sf::Text> m_pauseMenuItems{};
        sf::Text resumeText{};
        sf::Text saveGameText{};
        sf::Text exitGameText{};

        EconomyManager* m_economyManager{ nullptr };
        EnemyManager* m_enemyManager{ nullptr };
        Level* m_level{ nullptr };
        sf::RenderWindow* m_window{ nullptr };

        std::string getResourceTextString(int index, const EconomyManager* economyManager) const;

        sf::Vector2f getResourceTextPosition(int index, int windowWidth, int height) const;

        std::string getPauseMenuTextString(int index) const;

        sf::Vector2f getPauseMenuTextPosition(int index, int width, int height) const;

        bool interfaceLogic(sf::RenderWindow* window);

        bool executeLogicAction(int index);

        void nextTurn();

        void updateTexts();

        void buildingLogic(sf::RenderWindow* window);
        bool checkBuildingMode();
};