#ifndef CAR_H
#define CAR_H

#include <vectors.h>

template <typename T>
class Circuit;

template <typename Units>
class Car {
public:
    Car(Units x = 0, Units y = 0, Units velocityX = 0, Units velocityY = 0);

    void amendVelocity(int acceleration);
    void update();
    Vector2d<Units> const &getPosition() const;
    Vector2d<Units> const &getNextPosition() const;

    bool collidesWith(Circuit<Units> const& circuit) const;
    void setPositionToClosestPointOnCircuit(Circuit<Units> const& circuit);
    void zeroSpeed();

    void initializPosition(Circuit<Units> const& circuit, int player = 0, int numPlayers = 1);

private:

    Vector2d<Units> position;
    Vector2d<Units> velocity;
};

#include "Car.inl"

#endif 
