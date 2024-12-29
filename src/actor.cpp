#include "headers.h"

void Actor::read(Script acts)
{
    for (auto& actions : acts)
    {
        for (auto& action : actions)
        {
            action.jumpTo(*this);
        }
    }
    script = acts;
}

void Actor::setPosition(Vec2 newPos)
{
    const Vec2 deltaPosition = newPos - position;
    position = newPos;
    for (auto& vertex : vertices)
        vertex = vertex + deltaPosition;
}

Actor Actor::reverse()
{
    Actor reversed = *this;
    for (auto& actions : reversed.script)
    {
        for (auto& action : actions)
        {
            action = action.reverse();
            action.jumpTo(reversed);
        }
        // std::reverse(actions.begin(), actions.end());
    }
    std::reverse(reversed.script.begin(), reversed.script.end());

    return reversed;
}