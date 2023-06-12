#pragma once
#include <vector>

#include <vectors.h>

template <typename T>
struct WayPoint {
    Vector2d<T> center;

    float alpha;    // orientation

    int /*?*/ width;// width of the road at the way point (will always be 10 for now. Maybe should be a float.)
};

template <typename T>
class Circuit {
public:
    Circuit(std::vector<WayPoint<T>> wps) : mWaypoints(wps) {}

    void render() const
#ifdef USE_MATHLIB
        ;
#else
    {
        auto renderFunctor = [](Vector2 const& previousPosition, Vector2 const& position) {
            DrawLineEx(previousPosition, position, 3.f, BLACK);
            return false;
        };
        run(renderFunctor);
    }
#endif
   
    Vector2d<T>
    getStartPosition(int player = 0, int numPlayers = 1) const;
    bool intersect(Vector2d<T> const &point1, Vector2d<T> const &point2) const;

private:
    std::vector<WayPoint<T>> mWaypoints;
    template <class Functor> void run(Functor& f) const;
};

extern Circuit<int> circuit;
