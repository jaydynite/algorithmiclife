#include "Game.h"

//Private functions (initialisers)
void Game::initVariables()
{
    this->window = nullptr;
    this->leftClicked = false;
    this->leftClickedPrevious = false;
    this->rightClicked = false;
    this->rightClickedPrevious = false;
    this->shiftClicked = false;
    this->paused = true;

    //promptbox cell values init
    this->promptBoxActivated = false;
    if (!this->font.loadFromFile("/Users/jaydynite/Documents/GitHub/algorithmiclife/ARIAL.ttf"))
    {
        std::cerr << "Error loading font!" << std::endl;
        exit(EXIT_FAILURE); // Handle the error appropriately
    }
    this->currentInputIndex = 0;
    this->inputText = {"10.", "10.", "10.", "0.", "0."};
    lastCellSize = std::stof(this->inputText[0]);
    lastCellCharge = std::stof(this->inputText[1]);
    lastCellMass = std::stof(this->inputText[2]);
    lastCellVelocity = {std::stof(this->inputText[3]), std::stof(this->inputText[4])};
}

void Game::initWindow()
{
    this->videoMode.width = 1000;
    this->videoMode.height = 1000;
    this->window = new sf::RenderWindow(this->videoMode, "Algorithmic Life", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(10);
}

//(Conways:) Initialses 2d cells vector with all false (dead)
// void Game::initCells()
// {
//     this->cells.resize(cellsWidth + 2, std::vector<Cell>(cellsWidth + 2, Cell()));
//     this->nextGenCells.resize(cellsWidth + 2, std::vector<Cell>(cellsWidth + 2, Cell()));
// }

//Constructors
Game::Game()
{
    //(Conways) this->initCells();
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
        if (shiftClicked)
        {
            this->tempCellX = floor((this->mousePositionWindow.x)/20)*20;
            this->tempCellY = floor((this->mousePositionWindow.y)/20)*20;
        }
        else
        {
            this->tempCellX = this->mousePositionWindow.x;
            this->tempCellY = this->mousePositionWindow.y;
        }

        // Add the new cell with these attributes
        this->cells.push_back(Cell(lastCellSize, lastCellCharge, lastCellMass, {this->tempCellX - this->lastCellSize, this->tempCellY - this->lastCellSize}, {lastCellVelocity[0], lastCellVelocity[1]}, {0.f, 0.f}, {0.f, 0.f}));
    }
    else if (paused == true && this->rightClicked && !this->rightClickedPrevious)
    {
        if (shiftClicked)
        {
            this->tempCellX = floor((this->mousePositionWindow.x)/20)*20;
            this->tempCellY = floor((this->mousePositionWindow.y)/20)*20;
        }
        else
        {
            this->tempCellX = this->mousePositionWindow.x;
            this->tempCellY = this->mousePositionWindow.y;
        }

        this->promptBoxActivated = true;
        this->inputText = {"", "", "", "", ""};
        this->currentInputIndex = 0;
    }

    this->leftClickedPrevious = this->leftClicked;
    this->rightClickedPrevious = this->rightClicked;
}

// (Conways) void Game::updateCellsConways()
// {
//     //Loops through surrounding cells to detect alive neightbours and calculates the survival of each cell
//     for (int i = 1; i <= this->cellsHeight; i++)
//     {
//         for (int j = 1; j <= this->cellsWidth; j++)
//         {
//             int neighbourCount = 0;

//             if (this->cells[i - 1][j - 1].getAlive())
//                 neighbourCount++;
//             if (this->cells[i - 1][j].getAlive())
//                 neighbourCount++;
//             if (this->cells[i - 1][j + 1].getAlive())
//                 neighbourCount++;
//             if (this->cells[i][j - 1].getAlive())
//                 neighbourCount++;
//             if (this->cells[i][j + 1].getAlive())
//                 neighbourCount++;
//             if (this->cells[i + 1][j - 1].getAlive())
//                 neighbourCount++;
//             if (this->cells[i + 1][j].getAlive())
//                 neighbourCount++;
//             if (this->cells[i + 1][j + 1].getAlive())
//                 neighbourCount++;
            
//             if (this->cells[i][j].getAlive())
//             {
//                 this->nextGenCells[i][j].setAlive(neighbourCount == 2 || neighbourCount == 3);
//             }
//             else
//             {
//                 this->nextGenCells[i][j].setAlive(neighbourCount == 3);
//             }
//         }
//     }

//     std::swap(this->cells, this->nextGenCells);
// }

//Implements force interactions with coulomb's law.
void Game::updateCellsJays()
{
    for (int i = 0; i < this->cells.size(); i++)
    {
        this->cells[i].setForce({0.f, 0.f});

        for (int j = 0; j < this->cells.size(); j++)
        {
            if (j == i)
                continue;

            float distanceX = (this->cells[i].getPosition()[0] - this->cells[j].getPosition()[0]);
            float distanceY = (this->cells[i].getPosition()[1] - this->cells[j].getPosition()[1]);

            float distanceSquared = distanceX*distanceX + distanceY*distanceY;

            if (distanceSquared == 0)
                continue;
            
            float distance = sqrt(distanceSquared);
            float chargeProduct = cells[i].getCharge()*cells[j].getCharge();

            float chargeForceMagnitude = chargeProduct/distanceSquared;

            std::vector<float> chargeForce = {10000*chargeForceMagnitude*(distanceX/distance), 10000*chargeForceMagnitude*(distanceY/distance)};

            this->cells[i].addForce(chargeForce);


            
        }
        this->cells[i].updateAcceleration();
        this->cells[i].updateVelocity();
    }
}

//Updates cell velocities according to force interactions.
void Game::updateCellsPosition()
{
    for (int i = 0; i < this->cells.size(); i++)
    {
        this->cells[i].updatePosition();
    }
}

void Game::renderPromptBox()
{
    float promptBoxWidth = 400;
    float promptBoxHeight = 200;
    sf::RectangleShape promptBox(sf::Vector2f(promptBoxWidth, promptBoxHeight));
    promptBox.setFillColor(sf::Color::Yellow);
    promptBox.setPosition(this->videoMode.width/2 - promptBoxWidth/2, this->videoMode.height/2 - promptBoxHeight/2);
    this->window->draw(promptBox);

    // Render text fields for attributes
    sf::Text attributeText;
    attributeText.setFont(this->font);
    attributeText.setCharacterSize(20);
    attributeText.setFillColor(sf::Color::Black);

    std::vector<std::string> labels = {"Size: ", "Charge: ", "Mass: ", "Velocity X: ", "Velocity Y: "};
    for (int i = 0; i < this->inputText.size(); i++)
    {
        attributeText.setString(labels[i] + this->inputText[i]);
        attributeText.setPosition(320, 420 + i * 40);
        this->window->draw(attributeText);
    }
}

//Renders the alive cells in cells grid
void Game::renderCells()
{
    //Loops through cells to detect alive cells and displays them
    for (int i = 0; i < this->cells.size(); i++)
    {
        sf::CircleShape cellForm;
        cellForm.setPosition(this->cells[i].getPosition()[0], this->cells[i].getPosition()[1]);
        cellForm.setRadius(this->cells[i].getSize());
        cellForm.setFillColor(this->cells[i].getCharge() > 0.f? sf::Color::Cyan
                            : sf::Color::Magenta);

        this->window->draw(cellForm);
    }
}

void Game::resetCells()
{
    this->cells = std::vector<Cell>();
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
                if (!promptBoxActivated && this->evnt.key.code == sf::Keyboard::Escape)
                    this->window->close();
                else if (this->evnt.key.code == sf::Keyboard::Space)
                    this->paused = !this->paused;
                else if (this->evnt.key.code == sf::Keyboard::R)
                {
                    cells.pop_back();
                }
                else if (shiftClicked && this->evnt.key.code == sf::Keyboard::R)
                {
                    resetCells();
                }
                else if (this->evnt.key.code == sf::Keyboard::LShift)
                {
                    this->shiftClicked = true;
                }

                //Promptbox keypressed conditions
                else if (promptBoxActivated && this->evnt.key.code == sf::Keyboard::Escape)
                {
                    // Cancel input
                    this->promptBoxActivated = false;
                }
                // Backspace
                else if (promptBoxActivated && this->evnt.key.code == sf::Keyboard::BackSpace && !this->inputText[currentInputIndex].empty())
                {
                    this->inputText[currentInputIndex].pop_back();
                }
                else if (promptBoxActivated && this->evnt.key.code == sf::Keyboard::Enter)
                {
                    // Confirm input
                    // d means default. The previous stats are used.
                    if (this->inputText[0] != "d")
                    {
                        lastCellSize = std::stof(this->inputText[0]);
                    }
                    if (this->inputText[1] != "d")
                    {
                        lastCellCharge = std::stof(this->inputText[1]);
                    }
                    if (this->inputText[2] != "d")
                    {
                        lastCellMass = std::stof(this->inputText[2]);
                    }
                    if (this->inputText[3] != "d")
                    {
                        lastCellVelocity[0] = std::stof(this->inputText[3]);
                    }
                    if (this->inputText[4] != "d")
                    {
                        lastCellVelocity[1] = std::stof(this->inputText[4]);
                    }
                    
                    // Add the new cell with these attributes
                    this->cells.push_back(Cell(lastCellSize, lastCellCharge, lastCellMass, {this->tempCellX - this->lastCellSize, this->tempCellY - this->lastCellSize}, {lastCellVelocity[0], lastCellVelocity[1]}, {0.f, 0.f}, {0.f, 0.f}));

                    this->promptBoxActivated = false;  // Close the prompt box
                }
                else if (promptBoxActivated && this->evnt.key.code == sf::Keyboard::Tab)
                {
                    // Move to the next input field
                    currentInputIndex = (currentInputIndex + 1) % this->inputText.size();
                }
                break;
            case sf::Event::KeyReleased:
                if (this->evnt.key.code == sf::Keyboard::LShift)
                {
                    this->shiftClicked = false;
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
            case sf::Event::TextEntered:
                // Printable characters
                if (promptBoxActivated && this->evnt.text.unicode >= 32 && this->evnt.text.unicode <= 126)
                {
                    this->inputText[currentInputIndex] += this->evnt.text.unicode;
                }
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
    {
        this->updateCellsJays();
        this->updateCellsPosition();
        //std::cout << "Force: {" << (double)this->cells[0].getPosition()[0] << ", " << this->cells[0].getPosition()[1] << "}";
    }
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

    if (this->promptBoxActivated)
    {
        this->renderPromptBox();
    }

    //Displays what has been rendered so far
    this->window->display();
}