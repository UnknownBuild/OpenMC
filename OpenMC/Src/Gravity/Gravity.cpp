#include "Gravity.h"

Gravity::Gravity()
{
    this->able = true;
    this->a = 30.0f;
    this->velocity = 0.0f;
}

Gravity::~Gravity()
{
}

float Gravity::getVelocity()
{
    return this->velocity;
}

void Gravity::setVelocity(float velocity)
{
    this->velocity = velocity;
}

float Gravity::UpdateVelocity(float deltaTime)
{
    deltaTime /= 2;
    this->velocity = this->velocity - a * deltaTime;
    return this->velocity * deltaTime;
}
