#include <cassert>
#include <fstream>
#include <filesystem>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <Core/Game.h>
#include <Core/World.h>
#include <Utils/Constants.h>
#include <Utils/Tools.h>

bool Game::init(GameCreateInfo& createInfo)
{
	assert(m_window == nullptr && m_world == nullptr && "Game is already initialized, we are about to leak memory");

	m_window = new sf::RenderWindow({ createInfo.screenWidth, createInfo.screenHeight }, createInfo.gameTitle);
	m_window->setFramerateLimit(createInfo.frameRateLimit);
    m_view = m_window->getDefaultView();

    m_view.setCenter(320, 160);
    m_view.setSize(640.f, 320.f);
    m_window->setView(m_view);


	m_world = new World();
	const bool loadOk = m_world->load();

	return loadOk;
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


	// Update scene here
	m_world->update(deltaMilliseconds);
}

void Game::render()
{
	m_window->clear();

	m_world->render(*m_window);

	m_window->display();
}


void createDefaultConfigFile(std::string configPath)
{
    std::string gameTitle = GAME_TITLE;
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
    configFile << "GameTitle=" << gameTitle << std::endl;
    configFile << "ScreenWidth=" << screenWidth << std::endl;
    configFile << "ScreenHeight=" << screenHeight << std::endl;
    configFile << "FrameRateLimit=" << frameRateLimit << std::endl;

    configFile.close();
}

Game::GameCreateInfo loadFromConfig(std::string configPath)
{
    Game::GameCreateInfo gameInfo;

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
        // Check for each setting
        if (line.find("GameTitle=") == 0)
        {
            gameInfo.gameTitle = line.substr(10);
        }
        else if (line.find("ScreenWidth=") == 0)
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