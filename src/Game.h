#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

//Class that acts as the game engine: wrapper class.
class Game
{
    private:
        //Variables
        //Window
        sf::RenderWindow* window;
        sf::VideoMode videoMode;
        sf::Event evnt;

        //Mouse positions
        sf::Vector2i mousePositionWindow;

        //Mouse/key state
        bool leftClicked;
        bool leftClickedPrevious;
        bool paused;

        //Game objects
        std::vector<std::vector<bool>> cells;
        std::vector<std::vector<bool>> nextGenCells;
        int cellsWidth;
        int cellsHeight;
        int cellSize;

        //Private functions
        void initVariables();
        void initWindow();
        void initCells();

    public:
        //Constructor
        Game();
        //Destructor
        virtual ~Game();

        //Accessors
        bool running() const;

        //Member functions
        //Cell functions
        void spawnCells();
        void updateCells();
        void renderCells();
        void resetCells();

        void pollEvents();
        void updateMousePositionWindow();
        void update();
        void render();
};