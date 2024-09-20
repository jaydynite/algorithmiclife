#include <iostream>
#include "Game.h"

const float UPDATE_RATE = 1.0f/100000.0f;
const int UPDATES_PER_RENDER = 20;

int main()
{
    //Init Game engine
    Game game;

    sf::Clock clock;
    float accumulator = 0.0f;
    int updateCount = 0;

    //Game loop
    while (game.running())
    {
        float frameTime = clock.restart().asSeconds();
        accumulator += frameTime;

        //Update
        while (accumulator >= UPDATE_RATE)
        {
            game.update();
            accumulator -= UPDATE_RATE;
            updateCount++;

            //Render
            if (updateCount >= UPDATES_PER_RENDER)
            {
                game.render();
                updateCount = 0;
            }
        }
    }
}