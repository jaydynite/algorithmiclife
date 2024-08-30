#include "Cell.h"

//Initialisers
//Initialises cell with all specified attributes.
void Cell::initStates(float size,
                     float charge,
                     float mass,
                     std::vector<float> position,
                     std::vector<float> velocity,
                     std::vector<float> acceleration,
                     std::vector<float> force)
{
    this->size = size;
    this->charge = charge;
    this->mass = mass;
    this->position = position;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->force = force;
}

//Initialises empty cell.
void Cell::initStates()
{
    this->size = 0.f;
    this->charge = 0.f;
    this->mass = 0.f;
    this->position = {0.f, 0.f};
    this->velocity = {0.f, 0.f};
    this->acceleration = {0.f, 0.f};
    this->force = {0.f, 0.f};
}

//Constructor for all parameters
Cell::Cell(float size,
             float charge,
             float mass,
             std::vector<float> position,
             std::vector<float> velocity,
             std::vector<float> acceleration,
             std::vector<float> force)
{
    this->initStates(size,
                     charge,
                     mass,
                     position,
                     velocity,
                     acceleration,
                     force);
}

//Constructor for black cell
Cell::Cell()
{
    this->initStates();
}

//Destructor
Cell::~Cell()
{
    
}

//Getters
float Cell::getSize()
{
    return this->size;
}

float Cell::getCharge()
{
    return this->charge;
}

float Cell::getMass()
{
    return this->mass;
}

std::vector<float> Cell::getPosition()
{
    return this->position;
}

std::vector<float> Cell::getVelocity()
{
    return this->velocity;
}

std::vector<float> Cell::getAcceleration()
{
    return this->acceleration;
}

std::vector<float> Cell::getForce()
{
    return this->force;
}

//Settters
void Cell::setSize(float size)
{
    this->size = size;
}

void Cell::setCharge(float charge)
{
    this->charge = charge;
}

void Cell::setMass(float mass)
{
    this->mass = mass;
}

void Cell::setPosition(std::vector<float> position)
{
    this->position = position;
}

void Cell::setVelocity(std::vector<float> velocity)
{
    this->velocity = velocity;
}

void Cell::setAcceleration(std::vector<float> acceleration)
{
    this->acceleration = acceleration;
}

void Cell::setForce(std::vector<float> force)
{
    this->force = force;
}

void Cell::addPosition(std::vector<float> additionalPosition)
{
    for (int i = 0; i < 2; i++)
    {
        this->position[i] += additionalPosition[i];
    }
}

void Cell::addVelocity(std::vector<float> additionalVelocity)
{
    for (int i = 0; i < 2; i++)
    {
        this->velocity[i] += additionalVelocity[i];
    }
}

void Cell::addAcceleration(std::vector<float> additionalAcceleration)
{
    for (int i = 0; i < 2; i++)
    {
        this->acceleration[i] += additionalAcceleration[i];
    }
}

void Cell::addForce(std::vector<float> additionalForce)
{
    for (int i = 0; i < 2; i++)
    {
        this->force[i] += additionalForce[i];
    }
}

void Cell::updatePosition()
{
    this->addPosition(this->velocity);
}

void Cell::updateVelocity()
{
    this->addVelocity(this->acceleration);
}

void Cell::updateAcceleration()
{
    this->acceleration = {this->force[0]/this->mass, this->force[1]/this->mass};
}