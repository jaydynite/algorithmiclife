#include "Game.h"

//Private functions (initialisers)
void Game::initVariables()
{
    this->window = nullptr;
    this->leftClicked = false;
    this->leftClickedPrevious = false;
    this->paused = false;
}

void Game::initWindow()
{
    this->videoMode.width = this->cellsWidth*this->cellSize;
    this->videoMode.height = this->cellsHeight*this->cellSize;
    this->window = new sf::RenderWindow(this->videoMode, "Algorithmic Life", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(140);
}

//Initialses 2d cells vector with all false (dead)
void Game::initCells()
{
    this->cellsWidth = 100;
    this->cellsHeight = 100;
    this->cellSize = 10;

    this->cells.resize(cellsWidth + 2, std::vector<bool>(cellsWidth + 2, false));
    this->nextGenCells.resize(cellsWidth + 2, std::vector<bool>(cellsWidth + 2, false));
}

//Constructors
Game::Game()
{
    this->initCells();
    this->initVariables();
    this->initWindow();
}

//Destructors
Game::~Game()
{
    delete this->window;
}

//Accessors
bool Game::running() const
{
    return this->window->isOpen();
}

//Member functions
//Cell functions

//Spawns cells according to mouth position. Adjusts the position so that it fits in a 100x100 grid.
void Game::spawnCells()
{
    if (this->leftClicked && !this->leftClickedPrevious)
    {
        int x = floor(this->mousePositionWindow.x/10) + 1;
        int y = floor(this->mousePositionWindow.y/10) + 1;
        cells[x][y] = !cells[x][y];
    }

    this->leftClickedPrevious = this->leftClicked;
}

void Game::updateCells()
{
    //Loops through surrounding cells to detect alive neightbours and calculates the survival of each cell
    for (int i = 1; i <= this->cellsHeight; i++)
    {
        for (int j = 1; j <= this->cellsWidth; j++)
        {
            int neighbourCount = 0;

            if (this->cells[i - 1][j - 1])
                neighbourCount++;
            if (this->cells[i - 1][j])
                neighbourCount++;
            if (this->cells[i - 1][j + 1])
                neighbourCount++;
            if (this->cells[i][j - 1])
                neighbourCount++;
            if (this->cells[i][j + 1])
                neighbourCount++;
            if (this->cells[i + 1][j - 1])
                neighbourCount++;
            if (this->cells[i + 1][j])
                neighbourCount++;
            if (this->cells[i + 1][j + 1])
                neighbourCount++;
            
            if (this->cells[i][j])
            {
                this->nextGenCells[i][j] = (neighbourCount == 2 || neighbourCount == 3);
            }
            else
            {
                this->nextGenCells[i][j] = neighbourCount == 3;
            }
        }
    }

    std::swap(this->cells, this->nextGenCells);
}

//Renders the alive cells in cells grid
void Game::renderCells()
{
    //Loops through cells to detect alive cells and displays them
    for (int i = 1; i <= cellsHeight; i++)
    {
        for (int j = 1; j <= cellsWidth; j++)
        {
            if (cells[i][j])
            {
                sf::RectangleShape cellForm;
                cellForm.setPosition((i - 1)*cellSize, (j - 1)*cellSize);
                cellForm.setSize(sf::Vector2f((float)cellSize, (float)cellSize));
                cellForm.setFillColor(sf::Color::Cyan);

                this->window->draw(cellForm);
            }
        }
    }
}

void Game::resetCells()
{
    this->cells = std::vector<std::vector<bool>>(cellsWidth + 2, std::vector<bool>(cellsWidth + 2, false));
}

void Game::pollEvents()
{
    //Event polling
    while (this->window->pollEvent(this->evnt))
    {
        //Tests which buttons are pressed
        switch (this->evnt.type)
        {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyPressed:
                if (this->evnt.key.code == sf::Keyboard::Escape)
                    this->window->close();
                else if (this->evnt.key.code == sf::Keyboard::Space)
                    this->paused = !this->paused;
                else if (this->evnt.key.code == sf::Keyboard::R)
                {
                    resetCells();
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (this->evnt.mouseButton.button == sf::Mouse::Button::Left)
                    this->leftClicked = true;
                break;
            case sf::Event::MouseButtonReleased:
                if (this->evnt.mouseButton.button == sf::Mouse::Button::Left)
                    this->leftClicked = false;
                break;
            default:
                break;
        }
    }
}

/*
Updates the mouse positions relative to window
*/
void Game::updateMousePositionWindow()
{
    this->mousePositionWindow = sf::Mouse::getPosition(*this->window);
}

void Game::update()
{
    this->pollEvents();
    this->updateMousePositionWindow();
    this->spawnCells();
    if (!this->paused)
        this->updateCells();
}

/* 
Clears window
Renders window
*/
void Game::render()
{
    this->window->clear();

    //Render game objects
    this->renderCells();

    //Displays what has been rendered so far
    this->window->display();
}