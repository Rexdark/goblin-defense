
#include <cstdio>
#include <Core/Game.h>
#include <SFML/System/Clock.hpp>
#include <Utils/Constants.h>
#include <Utils/Tools.h>

int main()
{

    Game::GameCreateInfo gameCI = loadFromConfig(CONFIG_PATH);

    Game game;
    const bool gameInitialized = game.init(gameCI);

    if (gameInitialized == false)
    {
        printf("\nGame could not be initializedª\n");
        return 0;
    }
    else
    {
        printf("\nStarting game!\n\n####################\n\n");
    }

    sf::Clock clock;

    while (game.isRunning())
    {
        const sf::Time elapsed = clock.restart();

        game.update(elapsed.asMilliseconds());
        game.render();
    }
}