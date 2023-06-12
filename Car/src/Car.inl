#pragma once

template <typename Units>
Car<Units>::Car(Units x, Units y, Units velocityX, Units velocityY)
    : position(x, y), velocity(velocityX, velocityY) {}

template <typename Units>
void Car<Units>::amendVelocity(int acceleration) {
    switch (acceleration) {
    case 7:
        velocity.x -= 1;
        velocity.y -= 1;
        break;
    case 8:
        velocity.y -= 1;
        break;
    case 9:
        velocity.x += 1;
        velocity.y -= 1;
        break;
    case 4:
        velocity.x -= 1;
        break;
    case 6:
        velocity.x += 1;
        break;
    case 1:
        velocity.x -= 1;
        velocity.y += 1;
        break;
    case 2:
        velocity.y += 1;
        break;
    case 3:
        velocity.x += 1;
        velocity.y += 1;
        break;
    default:
        break;
    }
}

template <typename Units>
void Car<Units>::update() {
    position += velocity;
}

template <typename Units>
Vector2d<Units> const& Car<Units>::getPosition() const {
    return position;
}

template <typename Units>
Vector2d<Units> const& Car<Units>::getNextPosition() const {
    return position + velocity;
}

template <typename Units>
bool Car<Units>::collidesWith(Circuit<Units> const& circuit) const
{
    Vector2d<Units> nextPosition = position + velocity;

    return circuit.intersect(position, nextPosition);
}

template <typename Units>
void Car<Units>::setPositionToClosestPointOnCircuit(Circuit<Units> const& circuit)
{

}

template <typename Units>
void Car<Units>::zeroSpeed() {
    velocity = Vector2d<Units>();
}

template <typename Units>
void Car<Units>::initializPosition(Circuit<Units> const& circuit, int player/*= 0 */, int numPlayers/* = 1*/)
{
    position = circuit.getStartPosition(player, numPlayers);
}
