#ifndef CAR_H
#define CAR_H

#include <vectors.h>

template <typename T>
class Circuit;

class Graphics;

template <typename Units>
class Car {
public:
    Car(Units x = 0, Units y = 0, Units velocityX = 0, Units velocityY = 0);

    void amendVelocity(int acceleration);
    void update(Circuit<Units> const& circuit);
    Vector2d<Units> const& getPosition() const;
    Vector2d<Units> const& getVelocity() const { return velocity; }
    Vector2d<Units>        getNextPosition() const;
    float getOrientation() const
    {
        return - /* reference is left-handed ? */ atan2f(velocity.y, velocity.x) * RAD2DEG;
    }

    bool collidesWith(Circuit<Units> const& circuit, Vector2d<float>& positionOfImpact) const;
    void setPositionToClosestPointOnCircuit(Circuit<Units> const& circuit);
    void zeroSpeed();
    bool isFinished() const {
        return eState_finished == state;
    }
    void initializPosition(Circuit<Units> const& circuit, int player = 0, int numPlayers = 1);

    std::unique_ptr<Graphics>  graphics;

private:
    
    Vector2d<Units> position;
    Vector2d<Units> velocity;
    enum eState {
        eState_starting,
        eState_halfway,
        eState_finished,
    } state;
};

#include "Car.inl"

#endif 
