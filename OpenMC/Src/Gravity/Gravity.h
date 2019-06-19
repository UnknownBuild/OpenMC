#pragma once

class Gravity
{
public:
    Gravity();
    ~Gravity();

    float getVelocity();
    void setVelocity(float velocity);
    float UpdateVelocity(float deltaTime);

private:
    bool able;
    float a;
    float velocity;
};
