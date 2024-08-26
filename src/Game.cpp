#include "Game.h"

//Private functions (initialisers)
void Game::initVariables()
{
    this->window = nullptr;
    this->leftClicked = false;
    this->leftClickedPrevious = false;
    this->rightClicked = false;
    this->rightClickedPrevious = false;
    this->paused = true;
}

void Game::initWindow()
{
    this->videoMode.width = this->cellsWidth*this->cellSize;
    this->videoMode.height = this->cellsHeight*this->cellSize;
    this->window = new sf::RenderWindow(this->videoMode, "Algorithmic Life", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(500);
}

//Initialses 2d cells vector with all false (dead)
void Game::initCells()
{
    this->cellsWidth = 100;
    this->cellsHeight = 100;
    this->cellSize = 10;

    this->cells.resize(cellsWidth + 2, std::vector<Cell>(cellsWidth + 2, Cell()));
    this->nextGenCells.resize(cellsWidth + 2, std::vector<Cell>(cellsWidth + 2, Cell()));
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
    if (this->paused == true && this->leftClicked && !this->leftClickedPrevious)
    {
        int x = floor(this->mousePositionWindow.x/10) + 1;
        int y = floor(this->mousePositionWindow.y/10) + 1;
        this->cells[x][y].setAlive(true);
        this->cells[x][y].setCharge(1.f);
        this->cells[x][y].setMass(1.f);
    }
    else if (paused == true && this->rightClicked && !this->rightClickedPrevious)
    {
        int x = floor(this->mousePositionWindow.x/10) + 1;
        int y = floor(this->mousePositionWindow.y/10) + 1;
        this->cells[x][y].setAlive(true);
        this->cells[x][y].setCharge(-1.f);
        this->cells[x][y].setMass(1.f);
    }

    this->leftClickedPrevious = this->leftClicked;
    this->rightClickedPrevious = this->rightClicked;
}

void Game::updateCellsConways()
{
    //Loops through surrounding cells to detect alive neightbours and calculates the survival of each cell
    for (int i = 1; i <= this->cellsHeight; i++)
    {
        for (int j = 1; j <= this->cellsWidth; j++)
        {
            int neighbourCount = 0;

            if (this->cells[i - 1][j - 1].getAlive())
                neighbourCount++;
            if (this->cells[i - 1][j].getAlive())
                neighbourCount++;
            if (this->cells[i - 1][j + 1].getAlive())
                neighbourCount++;
            if (this->cells[i][j - 1].getAlive())
                neighbourCount++;
            if (this->cells[i][j + 1].getAlive())
                neighbourCount++;
            if (this->cells[i + 1][j - 1].getAlive())
                neighbourCount++;
            if (this->cells[i + 1][j].getAlive())
                neighbourCount++;
            if (this->cells[i + 1][j + 1].getAlive())
                neighbourCount++;
            
            if (this->cells[i][j].getAlive())
            {
                this->nextGenCells[i][j].setAlive(neighbourCount == 2 || neighbourCount == 3);
            }
            else
            {
                this->nextGenCells[i][j].setAlive(neighbourCount == 3);
            }
        }
    }

    std::swap(this->cells, this->nextGenCells);
}

//Implements force interactions with coulomb's law.
void Game::updateCellsJays()
{
    for (int i = 1; i <= this->cellsHeight; i++)
    {
        for (int j = 1; j <= this->cellsWidth; j++)
        {
            this->cells[i][j].setForce({0.f, 0.f});

            for (int k = 1; k <= this->cellsHeight; k++)
            {
                for (int l = 1; l <= this->cellsWidth; l++)
                {
                    if (k == i && l == j)
                        continue;
                    
                    float distanceY = (i - k);
                    float distanceX = (j - l);
                    float distanceSquared = distanceX*distanceX + distanceY*distanceY;

                    if (distanceSquared == 0)
                        continue;
                    
                    float distance = sqrt(distanceSquared);
                    float chargeProduct = cells[i][j].getCharge()*cells[k][l].getCharge();

                    float forceMagnitude = chargeProduct/distanceSquared;

                    std::vector<float> force = {forceMagnitude*(distanceX/distance), forceMagnitude*(distanceY/distance)};

                    this->cells[i][j].setForce(force);
                }
            }

            this->cells[i][j].updateAcceleration();
        }
    }
}

//Updates cell velocities according to force interactions.
void Game::updateCellsVelocities()
{
    for (int i = 1; i <= this->cellsHeight; i++)
    {
        for (int j = 1; j <= this->cellsWidth; j++)
        {
            this->cells[i][j].updateVelocity();
        }
    }
}

//Renders the alive cells in cells grid
void Game::renderCells()
{
    //Loops through cells to detect alive cells and displays them
    for (int i = 1; i <= this->cellsHeight; i++)
    {
        for (int j = 1; j <= this->cellsWidth; j++)
        {
            if (this->cells[i][j].getAlive())
            {
                sf::RectangleShape cellForm;
                cellForm.setPosition((i - 1)*this->cellSize, (j - 1)*this->cellSize);
                cellForm.setSize(sf::Vector2f((float)this->cellSize, (float)this->cellSize));
                cellForm.setFillColor(this->cells[i][j].getCharge() == 1.f? sf::Color::Cyan
                                    : sf::Color::Magenta);

                this->window->draw(cellForm);
            }
        }
    }
}

void Game::resetCells()
{
    this->cells = std::vector<std::vector<Cell>>(cellsWidth + 2, std::vector<Cell>(cellsWidth + 2, Cell()));
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
                else if (this->evnt.mouseButton.button == sf::Mouse::Button::Right)
                    this->rightClicked = true;
                break;
            case sf::Event::MouseButtonReleased:
                if (this->evnt.mouseButton.button == sf::Mouse::Button::Left)
                    this->leftClicked = false;
                else if (this->evnt.mouseButton.button == sf::Mouse::Button::Right)
                    this->rightClicked = false;
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
        this->updateCellsJays();
    this->updateCellsVelocities();
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