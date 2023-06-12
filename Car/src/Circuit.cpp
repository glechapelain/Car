#include <array>

#include <vectors.h>
#include <raylib.h>

#include "Track.h"

std::vector<WayPoint<int>> idontunderstand = {
    WayPoint<int>{ { 90 , 150 },
      30, 150 },
    WayPoint<int>{ { 80 , 110 },
      20, 150 },
    WayPoint<int>{ { 70 , 70 },
      0, 150 },
    WayPoint<int>{ { 90 , 30 },
      280, 150 },
    WayPoint<int>{ { 120 , 30 },
      220, 150 },
    WayPoint<int>{ { 150 , 45 },
      280, 150 },
    WayPoint<int>{ { 204 , 27 },
      270, 150 },
    WayPoint<int>{ { 245 , 33 },
      265, 150 },
    WayPoint<int>{ { 245 , 33 },
      265, 150 },
    WayPoint<int>{ { 325, 40 },
      220, 150 },
    WayPoint<int>{ { 325, 95 },
      160, 150 },
    WayPoint<int>{ { 300, 125 },
      120, 150 },
    WayPoint<int>{ { 270, 160 },
      120, 150 },
    WayPoint<int>{ { 240, 170 },
      100, 150 },
    WayPoint<int>{ { 200, 180 },
      90, 150 },
    WayPoint<int>{ { 160, 200 },
      85, 150 },
    WayPoint<int>{ { 120, 180 },
      85, 150 },
};

Circuit<int> circuit(idontunderstand);

Vector2d<float> toVecFloat(Vector2d<int> a) { return Vector2d<float>(a.x, a.y); }

template <typename T> template< class ExecuteOnLine>
void Circuit<T>::run(ExecuteOnLine &doSomething) const
{
    for (int side = 0; side < 2; ++side)
    {
        bool previousPositionSet = false;
#ifdef USE_MATHLIB
        Vector2d<float>
#else
        Vector2
#endif
        previousPosition;
        float const sign = side ? 1.f : -1.f;
        for(size_t i = 0; i<=mWaypoints.size();++i)
        {
            WayPoint<int> const& wp = mWaypoints[i % mWaypoints.size()];
            Vector2d<float> center = toVecFloat(wp.center) * 2;
#ifdef USE_MATHLIB
            Vector2d<float> ref(cos(wp.alpha * 3.14 / 180), -sin(wp.alpha * 3.14 / 180) );
            Vector2d<float> position = center + ref * sign * wp.width / 5.f;
#else
            Vector2 ref = { cos(wp.alpha * 3.14 / 180), -sin(wp.alpha * 3.14 / 180) };
            Vector2 position = { center.x + sign * ref.x * wp.width / 5.f, center.y + sign * ref.y * wp.width / 5.f };
#endif

            if (previousPositionSet)
            {
                if (doSomething(previousPosition, position))
                    break;
            }
            previousPosition = position;
            previousPositionSet = true;
        }
    }
}

#ifdef USE_MATHLIB
template <typename T>
void Circuit<T>::render() const     // <- Delete-me.
{
    for (int side = 0; side < 2; ++side)
    {
        bool previousPositionSet = false;
        Vector2 previousPosition;
        float const sign = side ? 1.f : -1.f;
        for (WayPoint<int> const& wp : mWaypoints)
        {
            Vector2d<T> center = wp.center * 2;
            Vector2 ref = { cos(wp.alpha * 3.14 / 180), -sin(wp.alpha * 3.14 / 180) };
            Vector2 position = { center.x + sign * ref.x * wp.width / 5.f, center.y + sign * ref.y * wp.width / 5.f };

            if (previousPositionSet)
            {
                DrawLineEx(previousPosition, position, 3.f, BLACK);
            }
            previousPosition = position;
            previousPositionSet = true;
        }
    }
}
#endif


template <typename T>
Vector2d<T>
Circuit<T>::getStartPosition(int player/*= 0 */, int numPlayers/* = 1*/) const
{
    return mWaypoints[0].center / 10;
}

template <typename T>
bool
Circuit<T>::intersect(Vector2d<T> const& point1, Vector2d<T> const& point2) const
{
    struct {
        bool
        operator()(Vector2d<float> const& previousPosition, Vector2d<float> const& position)
        {
            result |= Intersect(previousPosition, position, point1 * 20., point2 * 20.);
            return result;
        }

        bool
        operator()(Vector2 const& previousPosition, Vector2 const& position) // <- Delete-me
        {
            return false;
        }

        bool result = false;
        Vector2d<float> point1, point2;
    } collisionFunctor;
    collisionFunctor.point1 = toVecFloat(point1);
    collisionFunctor.point2 = toVecFloat(point2);
    run(collisionFunctor);
    return collisionFunctor.result;
}


template void Circuit<int>::render() const;
template Vector2d<int> Circuit<int>::getStartPosition(int player /*= 0 */ , int numPlayers /* = 1 */ ) const;
template bool Circuit<int>::intersect(Vector2d<int> const& point1, Vector2d<int> const& point2) const;
