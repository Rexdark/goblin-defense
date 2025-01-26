#pragma once

#include <cstdint>
#include <string>
#include <SFML/Graphics/View.hpp>

namespace sf
{
	class RenderWindow;
}

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

		bool init(GameCreateInfo& createInfo);

		bool isRunning() const;

		void update(uint32_t deltaMilliseconds);
		void render();

	private:

		sf::RenderWindow* m_window{ nullptr };
		sf::View m_view;

		World* m_world{ nullptr };
};

void createDefaultConfigFile(std::string configPath);

Game::GameCreateInfo loadFromConfig(std::string configPath);