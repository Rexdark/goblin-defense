#include <cassert>
#include <fstream>
#include <filesystem>
#include <thread>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <Core/Game.h>
#include <Core/World.h>
#include <UI/MainMenu.h>
#include <Utils/AssetManager.h>
#include <Utils/Constants.h>
#include <Utils/Tools.h>

bool Game::init(GameCreateInfo& createInfo)
{
	assert(m_window == nullptr && "Game is already initialized, we are about to leak memory");

    AssetManager* assetmanager =  new AssetManager();

    m_assetManager = assetmanager->getInstance();

	m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
	m_window->setFramerateLimit(createInfo.frameRateLimit);
    m_view = m_window->getDefaultView();

    m_view.setCenter(320, 160);
    m_view.setSize(640.f, 320.f);
    m_window->setView(m_view);

    m_gameState = State_MainMenu;

	return true;
}

Game::~Game()
{
	m_world->unload();

	delete m_world;
	delete m_window;
}

bool Game::isRunning() const 
{ 
	return m_window->isOpen(); 
}

void Game::update(uint32_t deltaMilliseconds)
{
	// Check if user closed the window
    for (auto event = sf::Event(); m_window->pollEvent(event);)
    {
        if (event.type == sf::Event::Closed)
        {
            m_window->close();
        }
    }

    switch (m_gameState) {

        int option;

        case State_MainMenu:

            deleteWorld();

            if (!m_mainMenu)
            {
                printf("Instantiating new main menu.\n");
                m_mainMenu = new MainMenu(m_view.getSize().x, m_view.getSize().y, m_assetManager);
            }

            option = m_mainMenu->menuLogic(m_window);

            if (option != -1)
            {

                if (option == 0)
                {
                    newGame = true;
                    m_gameState = State_World;
                    deleteMainMenu();
                }
                if (option == 1)
                {
                    m_gameState = State_World;
                    deleteMainMenu();
                }
                if (option == 2)
                {
                    m_window->close();
                    exit(0);
                }
            }

            break;

        case State_World:

            if (!m_world)
            {
                m_world = new World();

                if (!m_world->load(newGame, m_view.getSize().x, m_view.getSize().y, m_assetManager))
                {
                    printf("Game world couldn't be loaded!");
                    m_window->close();
                }
            }

            if (m_world->update(deltaMilliseconds, m_window))
            {
                printf("Returning to main menu.\n");
                m_gameState = State_MainMenu;
                std::this_thread::sleep_for(std::chrono::milliseconds(250));
            }

            break;

        default:

            printf("CRITICAL ERROR: Unrecognized game state! Going back to main menu.");

            m_gameState = State_MainMenu;

            deleteMainMenu();
            deleteWorld();

            break;
    }
}

void Game::render()
{
    switch (m_gameState) {

        case State_MainMenu:

            m_window->clear();

            if (m_mainMenu)
            {
                m_mainMenu->draw(*m_window);
            }

            m_window->display();

            break;

        case State_World:
            
           m_window->clear(); 
           
           if (m_world)
           {
               m_world->render(*m_window);
           }

           m_window->display();

            break;

        default:

            printf("CRITICAL ERROR: Unrecognized game state! Going back to main menu.");

            delete m_mainMenu;
            delete m_world;

            m_gameState = State_MainMenu;

            break;
    }
}

void Game::deleteMainMenu()
{
    if (m_mainMenu)
    {
        delete m_mainMenu;
        m_mainMenu = { nullptr };
    }
}

void Game::deleteWorld()
{
    if (m_world)
    {
        m_world->unload();

        delete m_world;
        m_world = { nullptr };
    }

}


void createDefaultConfigFile(std::string configPath)
{
    //std::string gameTitle = GAME_TITLE;
    uint32_t screenWidth = SCREEN_WIDTH;
    uint32_t screenHeight = SCREEN_HEIGHT;
    uint32_t frameRateLimit = FRAMERATE;

    std::filesystem::path filePath(configPath);

    //Just in case folder might have been deleted by user
    if (!std::filesystem::exists(filePath.parent_path())) {
        std::filesystem::create_directories(filePath.parent_path());
    }

    std::ofstream configFile(filePath);
    if (!configFile.is_open()) {
        printf("Failed to create or open the config file.\n");
    }

    configFile << "[GameSettings]" << std::endl;
    //configFile << "GameTitle=" << gameTitle << std::endl;
    configFile << "ScreenWidth=" << screenWidth << std::endl;
    configFile << "ScreenHeight=" << screenHeight << std::endl;
    configFile << "FrameRateLimit=" << frameRateLimit << std::endl;

    configFile.close();
}

Game::GameCreateInfo loadFromConfig(std::string configPath)
{
    Game::GameCreateInfo gameInfo;

    gameInfo.gameTitle = GAME_TITLE;

    if (!fileExists(configPath))
    {
        printf("Config file does not exist! Creating config file with default options!\n");
        createDefaultConfigFile(configPath);
    }

    std::ifstream configFile(configPath);

    if (!configFile.is_open())
    {
        printf("Failed to open the config file!\n");
        return gameInfo;
    }

    std::string line;
    while (std::getline(configFile, line))
    {
        if (line.find("ScreenWidth=") == 0)
        {
            gameInfo.screenWidth = std::stoi(line.substr(12));
        }
        else if (line.find("ScreenHeight=") == 0)
        {
            gameInfo.screenHeight = std::stoi(line.substr(13));
        }
        else if (line.find("FrameRateLimit=") == 0)
        {
            gameInfo.frameRateLimit = std::stoi(line.substr(15));
        }
    }

    printf("\n######################################\n");
    printf("Game Config:\nTile: %s\nScreen Width: %dp\nScreen Height: %dp\nFramerate: %dfps\n", gameInfo.gameTitle.c_str(), gameInfo.screenWidth, gameInfo.screenHeight, gameInfo.frameRateLimit);
    printf("######################################\n\n");
    printf("Tip:You can change your desired resolution at /config/config.ini\n\n");

    configFile.close();

    return gameInfo;
}