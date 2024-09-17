#pragma once

#include <iostream>
#include <vector>

class Cell
{
    private:
        //Cell states
        float size;
        float charge;
        float mass;
        std::vector<float> position;
        std::vector<float> velocity;
        std::vector<float> acceleration;
        std::vector<float> force;
        std::vector<float> collisionImpulse;

        //Initialisers
        void initStates(float size,
                        float charge,
                        float mass,
                        std::vector<float> position,
                        std::vector<float> velocity,
                        std::vector<float> acceleration,
                        std::vector<float> force,
                        std::vector<float> collisionImpulse);

        void initStates();

    public:
        //Constructor and destructor
        Cell(float size,
             float charge,
             float mass,
             std::vector<float> position,
             std::vector<float> velocity,
             std::vector<float> acceleration,
             std::vector<float> force,
             std::vector<float> collisionImpulse);
        
        Cell();

        ~Cell();

        //Getters
        float getSize();
        float getCharge();
        float getMass();
        std::vector<float> getPosition();
        std::vector<float> getVelocity();
        std::vector<float> getAcceleration();
        std::vector<float> getForce();
        std::vector<float> getCollisionImpulse();

        //Setters
        void setSize(float size);
        void setCharge(float charge);
        void setMass(float mass);
        void setPosition(std::vector<float> position);
        void setVelocity(std::vector<float> velocity);
        void setAcceleration(std::vector<float> acceleration);
        void setForce(std::vector<float> force);
        void setCollisionImpulse(std::vector<float> collisionImpulse);

        void addPosition(std::vector<float> additionalPosition);
        void addVelocity(std::vector<float> additionalVelocity);
        void addAcceleration(std::vector<float> additionalAcceleration);
        void addForce(std::vector<float> additionalForce);
        void addCollisionImpulse(std::vector<float> additionalCollisionImpulse);

        void updatePosition();
        void updateVelocity();
        void updateAcceleration();
};