#pragma once

#include <iostream>
#include <cstdint>
#include <string>

#include <SFML/Graphics/View.hpp>
#include <Utils/AssetManager.h>


namespace sf
{
	class RenderWindow;
}

class MainMenu;

class World;

class Game
{
	public:

		struct GameCreateInfo
		{
			std::string gameTitle = "";
			uint32_t screenWidth = 0u;
			uint32_t screenHeight = 0u;
			uint32_t frameRateLimit = 0u;
		};

		~Game();

		AssetManager* m_assetManager{ nullptr };

		bool init(GameCreateInfo& createInfo);

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

	private:

		sf::RenderWindow* m_window{ nullptr };
		sf::View m_view;

		enum states { State_MainMenu, State_World };

		states m_gameState;

		MainMenu* m_mainMenu{ nullptr };
		World* m_world{ nullptr };

		bool newGame = false;

		void deleteMainMenu();
		void deleteWorld();
};

void createDefaultConfigFile(std::string configPath);

Game::GameCreateInfo loadFromConfig(std::string configPath);