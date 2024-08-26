#pragma once

#include <iostream>

class Cell
{
    private:
        //Cell states
        bool alive;
        float charge;
        float mass;
        std::vector<float> position;
        std::vector<float> velocity;
        std::vector<float> acceleration;
        std::vector<float> force;

        //Initialisers
        void initStates(bool alive,
                        float charge,
                        float mass,
                        std::vector<float> position,
                        std::vector<float> velocity,
                        std::vector<float> acceleration,
                        std::vector<float> force);

        void initStates();

    public:
        //Constructor and destructor
        Cell(bool alive,
             float charge,
             float mass,
             std::vector<float> position,
             std::vector<float> velocity,
             std::vector<float> acceleration,
             std::vector<float> force);
        
        Cell();

        ~Cell();

        //Reverse life state
        void reverseLife();

        //Getters
        bool getAlive();
        float getCharge();
        float getMass();
        std::vector<float> getPosition();
        std::vector<float> getVelocity();
        std::vector<float> getAcceleration();
        std::vector<float> getForce();

        //Setters
        void setAlive(bool alive);
        void setCharge(float charge);
        void setMass(float mass);
        void setPosition(std::vector<float> position);
        void setVelocity(std::vector<float> velocity);
        void setAcceleration(std::vector<float> acceleration);
        void setForce(std::vector<float> force);

        void addPosition(std::vector<float> additionalPosition);
        void addVelocity(std::vector<float> additionalVelocity);
        void addAcceleration(std::vector<float> additionalAcceleration);
        void addForce(std::vector<float> additionalForce);

        void updatePosition();
        void updateVelocity();
        void updateAcceleration();
};