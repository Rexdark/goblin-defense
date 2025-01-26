
#include <cstdio>
#include <Core/Game.h>
#include <SFML/System/Clock.hpp>
#include <Utils/Constants.h>
#include <Utils/Tools.h>

int main()
{
    Game game;

    Game::GameCreateInfo gameCI = loadFromConfig(CONFIG_PATH);

    game.init(gameCI);

    printf("\nStarting game!\n\n####################\n\n");

    sf::Clock clock;

    while (game.isRunning())
    {
        const sf::Time elapsed = clock.restart();

        game.update(elapsed.asMilliseconds());
        game.render();
    }
}