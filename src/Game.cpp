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
    //this->window->setFramerateLimit(10000);
}

void Game::initMainView()
{
    this->mainView.setSize(1000, 1000);
}

//Constructors
Game::Game()
{
    //(Conways) this->initCells();
    this->initVariables();
    this->initWindow();
    this->initMainView();
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
        // Convert mouse position from window coordinates to world coordinates
        sf::Vector2f worldPos = this->window->mapPixelToCoords(this->mousePositionWindow, this->mainView);

        if (shiftClicked)
        {
            this->tempCellX = floor((worldPos.x)/20)*20;
            this->tempCellY = floor((worldPos.y)/20)*20;
        }
        else
        {
            this->tempCellX = worldPos.x;
            this->tempCellY = worldPos.y;
        }

        // Add the new cell with these attributes
        this->cells.push_back(Cell(lastCellSize, lastCellCharge, lastCellMass, {this->tempCellX, this->tempCellY}, {lastCellVelocity[0], lastCellVelocity[1]}, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, 0.f));
    }
    else if (paused == true && this->rightClicked && !this->rightClickedPrevious)
    {
        // Convert mouse position from window coordinates to world coordinates
        sf::Vector2f worldPos = this->window->mapPixelToCoords(this->mousePositionWindow, this->mainView);

        if (shiftClicked)
        {
            this->tempCellX = floor((worldPos.x)/20)*20;
            this->tempCellY = floor((worldPos.y)/20)*20;
        }
        else
        {
            this->tempCellX = worldPos.x;
            this->tempCellY = worldPos.y;
        }

        this->promptBoxActivated = true;
        this->inputText = {"", "", "", "", ""};
        this->currentInputIndex = 0;
    }

    this->leftClickedPrevious = this->leftClicked;
    this->rightClickedPrevious = this->rightClicked;
}

//Implements force interactions with coulomb's law.
void Game::updateCells()
{
    for (int i = 0; i < this->cells.size(); i++)
    {
        this->cells[i].setForce({0.f, 0.f});
        this->cells[i].setCollisionImpulse({0.f, 0.f});

        for (int j = 0; j < this->cells.size(); j++)
        {
            if (j == i)
                continue;
            if (this->cells[i].getEnergy() == 0 && this->cells[j].getEnergy() == 0)
            {
                float distanceX = (this->cells[i].getPosition()[0] - this->cells[j].getPosition()[0]);
                float distanceY = (this->cells[i].getPosition()[1] - this->cells[j].getPosition()[1]);

                float distanceSquared = distanceX*distanceX + distanceY*distanceY;

                if (distanceSquared == 0)
                    continue;
                
                float distance = sqrt(distanceSquared);
                float chargeProduct = this->cells[i].getCharge()*this->cells[j].getCharge();
                
                float kConstant = 100;
                float chargeForceMagnitude = kConstant*chargeProduct/distanceSquared;

                std::vector<float> chargeForce = {chargeForceMagnitude*(distanceX/distance), chargeForceMagnitude*(distanceY/distance)};

                this->cells[i].addForce(chargeForce);

                //Collision force calculator
                if (distance <= this->cells[i].getSize() + this->cells[j].getSize())
                {
                    //Calculating normal unit vector of collision tangent
                    std::vector<float> normalUnitVector = {distanceX/distance, distanceY/distance};

                    //Relative velocity
                    std::vector<float> relativeVelocity = {this->cells[j].getVelocity()[0] - this->cells[i].getVelocity()[0],
                                                        this->cells[j].getVelocity()[1] - this->cells[i].getVelocity()[1]};
                    //Relative velocity along the normal
                    float speedAlongNormal = relativeVelocity[0]*normalUnitVector[0] + relativeVelocity[1]*normalUnitVector[1];
                    
                    if (speedAlongNormal < 0)
                        continue;
                    
                    //Currently all the force applied along the normal
                    //float forceAlongNormalMagnitude = this->cells[i].getForce()[0]*normalUnitVector[0] + relativeVelocity[1]*normalUnitVector[1];

                    //Calculate charge difference
                    float chargeDifference = std::abs(this->cells[i].getCharge() - this->cells[j].getCharge());
                    
                    //Calculate coefficient of restitution according to exponential decay
                    float chargeSensitivity = 0.05f;
                    float restitution = std::exp(-chargeSensitivity * chargeDifference);
                    
                    //Expected impulse magnitude calculation
                    float impulseMagnitude = -(1+restitution)*speedAlongNormal/(1/this->cells[i].getMass() + 1/this->cells[j].getMass());
                    
                    //Expected impulse
                    std::vector<float> impulse = {impulseMagnitude*normalUnitVector[0], impulseMagnitude*normalUnitVector[1]};

                    this->cells[i].addCollisionImpulse({-impulse[0], -impulse[1]});

                    // Apply position correction to prevent sinking
                    float overlap = this->cells[i].getSize() + this->cells[j].getSize() - distance;
                    std::vector<float> correction = {0.5f*overlap*normalUnitVector[0],
                                                    0.5f*overlap*normalUnitVector[1]};

                    this->cells[i].addPosition({correction[0], correction[1]});

                    //Photon emission calculation
                    float energyLost = 0.5f*(this->cells[i].getMass()*this->cells[j].getMass()/(this->cells[i].getMass() + this->cells[j].getMass())) * 
                                       (1 - restitution*restitution)*speedAlongNormal*speedAlongNormal;

                    // Create one photon with half the energy lost
                    if (energyLost > 0)
                    {
                        float photonEnergy = energyLost/2.0f;

                        // Calculate tangent vector
                        std::vector<float> tangentUnitVector = {-normalUnitVector[1], normalUnitVector[0]};

                        // Collision point
                        std::vector<float> collisionPoint = {(this->cells[i].getPosition()[0] + this->cells[j].getPosition()[0])/2,
                                                             (this->cells[i].getPosition()[1] + this->cells[j].getPosition()[1])/2};

                        // Create one photon
                        this->cells.push_back(Cell(1.f, 0.f, 0.f, collisionPoint, tangentUnitVector, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, photonEnergy));
                    }
                }
            }
        }
    }
}

//Updates cell velocities according to force interactions.
void Game::updateCellsPosition()
{
    for (int i = 0; i < this->cells.size(); i++)
    {
        if (this->cells[i].getEnergy() == 0)
        {
            //Boundary collisions
            //Left boundary
            if (this->cells[i].getPosition()[0] - this->cells[i].getSize() <= this->boundary.getPosition().x)
            {
                //Relative velocity of the boundary along the normal
                float relativeBoundarySpeedAlongNormal = -this->cells[i].getVelocity()[0];

                //Expected impulse magnitude calculation
                float impulseMagnitude = -2*relativeBoundarySpeedAlongNormal/(1/this->cells[i].getMass());

                this->cells[i].addCollisionImpulse({-impulseMagnitude, 0});
            }
            //Right boundary
            if (this->cells[i].getPosition()[0] + this->cells[i].getSize() >= this->boundary.getPosition().x + this->boundary.getSize().x)
            {
                //Relative velocity of the boundary along the normal
                float relativeBoundarySpeedAlongNormal = -this->cells[i].getVelocity()[0];

                //Expected impulse magnitude calculation
                float impulseMagnitude = -2*relativeBoundarySpeedAlongNormal/(1/this->cells[i].getMass());

                this->cells[i].addCollisionImpulse({-impulseMagnitude, 0});
            }
            //Top boundary
            if (this->cells[i].getPosition()[1] - this->cells[i].getSize() <= this->boundary.getPosition().y)
            {
                //Relative velocity of the boundary along the normal
                float relativeBoundarySpeedAlongNormal = -this->cells[i].getVelocity()[1];

                //Expected impulse magnitude calculation
                float impulseMagnitude = -2*relativeBoundarySpeedAlongNormal/(1/this->cells[i].getMass());

                this->cells[i].addCollisionImpulse({0, -impulseMagnitude});
            }
            //Bottom boundary
            if (this->cells[i].getPosition()[1] + this->cells[i].getSize() >= this->boundary.getPosition().y + this->boundary.getSize().y)
            {
                //Relative velocity of the boundary along the normal
                float relativeBoundarySpeedAlongNormal = -this->cells[i].getVelocity()[1];

                //Expected impulse magnitude calculation
                float impulseMagnitude = -2*relativeBoundarySpeedAlongNormal/(1/this->cells[i].getMass());

                this->cells[i].addCollisionImpulse({0, -impulseMagnitude});
            }

            //std::cout << "Cell " << i << " forces: " << "(" << this->cells[i].getForce()[0] << ", " << this->cells[i].getForce()[1] << ")\n";
            this->cells[i].updateAcceleration();

            if (this->cells[i].getCollisionImpulse()[0] == 0 && this->cells[i].getCollisionImpulse()[1] == 0)
                this->cells[i].updateVelocity();
            else
                this->cells[i].addVelocity({this->cells[i].getCollisionImpulse()[0]/this->cells[i].getMass(), this->cells[i].getCollisionImpulse()[1]/this->cells[i].getMass()});

            this->cells[i].updatePosition();
            //std::cout << "Cell " << i << " velocity: " << "(" << this->cells[i].getVelocity()[0] << ", " << this->cells[i].getVelocity()[1] << ")\n";

            float speed = sqrt(this->cells[i].getVelocity()[0]*this->cells[i].getVelocity()[0]
                            + this->cells[i].getVelocity()[1]*this->cells[i].getVelocity()[1]);
            
            if (speed >= 1)
            {
                float photonEnergy = this->cells[i].getMass();

                std::vector<float> directionUnitVector = {this->cells[i].getVelocity()[0]/speed, this->cells[i].getVelocity()[1]/speed};

                // Create one photon
                this->cells.push_back(Cell(1.f, 0.f, 0.f, this->cells[i].getPosition(), directionUnitVector, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, photonEnergy));

                //Destroy this cell
                this->cells.erase(this->cells.begin() + (i));
                i--;
            }
        }
        else
        {
            //Boundary collisions
            //Left boundary
            if (this->cells[i].getPosition()[0] - this->cells[i].getSize() <= this->boundary.getPosition().x)
            {
                //Relative velocity of the boundary along the normal
                float relativeBoundarySpeedAlongNormal = -this->cells[i].getVelocity()[0];

                //Expected impulse magnitude calculation
                float impulseMagnitude = -2*relativeBoundarySpeedAlongNormal;

                this->cells[i].addCollisionImpulse({-impulseMagnitude, 0});
            }
            //Right boundary
            if (this->cells[i].getPosition()[0] + this->cells[i].getSize() >= this->boundary.getPosition().x + this->boundary.getSize().x)
            {
                //Relative velocity of the boundary along the normal
                float relativeBoundarySpeedAlongNormal = -this->cells[i].getVelocity()[0];

                //Expected impulse magnitude calculation
                float impulseMagnitude = -2*relativeBoundarySpeedAlongNormal;

                this->cells[i].addCollisionImpulse({-impulseMagnitude, 0});
            }
            //Top boundary
            if (this->cells[i].getPosition()[1] - this->cells[i].getSize() <= this->boundary.getPosition().y)
            {
                //Relative velocity of the boundary along the normal
                float relativeBoundarySpeedAlongNormal = -this->cells[i].getVelocity()[1];

                //Expected impulse magnitude calculation
                float impulseMagnitude = -2*relativeBoundarySpeedAlongNormal;

                this->cells[i].addCollisionImpulse({0, -impulseMagnitude});
            }
            //Bottom boundary
            if (this->cells[i].getPosition()[1] + this->cells[i].getSize() >= this->boundary.getPosition().y + this->boundary.getSize().y)
            {
                //Relative velocity of the boundary along the normal
                float relativeBoundarySpeedAlongNormal = -this->cells[i].getVelocity()[1];

                //Expected impulse magnitude calculation
                float impulseMagnitude = -2*relativeBoundarySpeedAlongNormal;

                this->cells[i].addCollisionImpulse({0, -impulseMagnitude});
            }

            this->cells[i].addVelocity({this->cells[i].getCollisionImpulse()[0], this->cells[i].getCollisionImpulse()[1]});

            this->cells[i].updatePosition();
        }
    }
    //std::cout << "\n";
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
        cellForm.setPosition(this->cells[i].getPosition()[0] - this->cells[i].getSize(), this->cells[i].getPosition()[1] - this->cells[i].getSize());
        cellForm.setRadius(this->cells[i].getSize());
        cellForm.setFillColor(this->cells[i].getEnergy() > 0.f? sf::Color::Yellow
                            : this->cells[i].getCharge() > 0.f? sf::Color::Cyan
                            : this->cells[i].getCharge() == 0? sf::Color::White
                            : sf::Color::Magenta);

        this->window->draw(cellForm);
    }
}

void Game::renderBoundary()
{
    this->boundary.setSize(sf::Vector2f(1000.f, 1000.f));
    this->boundary.setPosition(0, 0);
    this->boundary.setFillColor(sf::Color::Transparent);
    this->boundary.setOutlineColor(sf::Color::White);
    this->boundary.setOutlineThickness(10.f);

    this->window->draw(boundary);
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
                if (!promptBoxActivated)
                {
                    if (this->evnt.key.code == sf::Keyboard::Escape)
                        this->window->close();
                    else if (this->evnt.key.code == sf::Keyboard::Space)
                        this->paused = !this->paused;
                    else if (this->evnt.key.code == sf::Keyboard::R)
                    {
                        if (shiftClicked)
                            resetCells();
                        else
                            cells.pop_back();
                    }
                    else if (this->evnt.key.code == sf::Keyboard::LShift)
                    {
                        this->shiftClicked = true;
                    }
                    else if (this->evnt.key.code == sf::Keyboard::W)
                    {
                        if (shiftClicked)
                            mainView.move(0.f, -0.01f*mainView.getSize().y);
                        else
                            mainView.move(0.f, -0.1f*mainView.getSize().y);
                    }
                    else if (this->evnt.key.code == sf::Keyboard::S)
                    {
                        if (shiftClicked)
                            mainView.move(0.f, 0.01f*mainView.getSize().y);
                        else
                            mainView.move(0.f, 0.1f*mainView.getSize().y);
                    }
                    else if (this->evnt.key.code == sf::Keyboard::A)
                    {
                        if (shiftClicked)
                            mainView.move(-0.01f*mainView.getSize().y, 0.f);
                        else
                            mainView.move(-0.1f*mainView.getSize().y, 0.f);
                    }
                    else if (this->evnt.key.code == sf::Keyboard::D)
                    {
                        if (shiftClicked)
                            mainView.move(0.01f*mainView.getSize().y, 0.f);
                        else
                            mainView.move(0.1f*mainView.getSize().y, 0.f);
                    }
                    else if (this->evnt.key.code == sf::Keyboard::Q)
                    {
                        if (shiftClicked)
                            mainView.zoom(1.5f);
                        else
                            mainView.zoom(2.f);
                    }
                    else if (this->evnt.key.code == sf::Keyboard::E)
                    {
                        if (shiftClicked)
                            mainView.zoom(0.75f);
                        else
                            mainView.zoom(0.5f);
                    }
                }
                else
                {
                    //Promptbox keypressed conditions
                    if (this->evnt.key.code == sf::Keyboard::Escape)
                    {
                        // Cancel input
                        this->promptBoxActivated = false;
                    }
                    // Backspace
                    else if (this->evnt.key.code == sf::Keyboard::BackSpace && !this->inputText[currentInputIndex].empty())
                    {
                        this->inputText[currentInputIndex].pop_back();
                    }
                    else if (this->evnt.key.code == sf::Keyboard::Enter)
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
                        this->cells.push_back(Cell(lastCellSize, lastCellCharge, lastCellMass, {this->tempCellX, this->tempCellY}, {lastCellVelocity[0], lastCellVelocity[1]}, {0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f}, 0.f));

                        this->promptBoxActivated = false;  // Close the prompt box
                    }
                    else if (this->evnt.key.code == sf::Keyboard::Tab)
                    {
                        // Move to the next input field
                        currentInputIndex = (currentInputIndex + 1) % this->inputText.size();
                    }
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
        this->updateCells();
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

    //Sets window as view (normal stuff)
    this->window->setView(this->mainView);

    //Render game objects that are not view centric.
    this->renderBoundary();
    this->renderCells();

    //Sets window as default (UI stuff)
    this->window->setView(window->getDefaultView());

    //Render game objects that are view centric (UI stuff)
    if (this->promptBoxActivated)
    {
        this->renderPromptBox();
    }

    //Displays what has been rendered so far
    this->window->display();
}