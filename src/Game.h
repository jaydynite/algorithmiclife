#pragma once

#include "Cell.h"

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
        bool rightClicked;
        bool rightClickedPrevious;
        bool shiftClicked;
        bool paused;
        bool promptBoxActivated;

        //Game objects
        std::vector<Cell> cells;
        std::vector<Cell> nextGenCells;
        float lastCellSize;
        float lastCellCharge;
        float lastCellMass;
        std::vector<float> lastCellVelocity;
        std::vector<float> lastCellAcceleration;
        std::vector<float> lastCellForce;


        //Private functions
        void initVariables();
        void initWindow();
        //(Conway) void initCells();
        
        //Prompt window state anc temp cell state
        std::vector<std::string> inputText;
        int currentInputIndex;
        sf::Font font;
        float tempCellX;
        float tempCellY;

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
        void updateCellsConways();
        void updateCellsJays();
        void updateCellsPosition();
        void renderCells();
        void resetCells();

        //update/render functions
        void pollEvents();
        void updateMousePositionWindow();
        void renderPromptBox();
        void handlePromptInput(sf::Event event);
        void update();
        void render();
};