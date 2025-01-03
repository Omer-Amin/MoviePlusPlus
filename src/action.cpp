#include "MPP.h"

std::function<Vec2(Actor&)> Action::arg(Vec2 vec)
{
    return [=](Actor actor) {
        return vec;
    };
}

std::function<Vec2(Actor&)> Action::arg(float x, float y)
{
    return [=](Actor actor) {
        return Vec2(x, y);
    };
}

std::vector<std::function<Vec2(Actor&)>> Action::getPath(std::vector<Vec2> points)
{
    std::vector<std::function<Vec2(Actor&)>> pointMethods;
    for (auto& p : points)
    {
        pointMethods.push_back(Action::arg(p));
    }

    return pointMethods;
}

void Action::jumpTo(Actor& actor, float point)
{
    path.clear();
    path.reserve(pathMethod.size());
    for (auto& method : pathMethod)
    {
        path.push_back(method(actor));
    }
    pivot = pivotMethod(actor);

    completion = point;
    if (isReversed)
    {
        transform(actor, 1 - easing(completion));
    }
    else
    {
        transform(actor, easing(completion));
    }
}

void Action::setFPS(unsigned int newFPS)
{
    fps = newFPS;
    frames = fps * duration;
    tickDelta = 1 / frames;
}

Action Action::reverse()
{
    Action reversed = *this;
    reversed.isReversed = true;
    return reversed;
}

float Action::tick(Actor& actor)
{
    if (completion < 1)
    {
        if (isReversed)
        {
            transform(actor, 1 - easing(completion));
        }
        else
        {
            transform(actor, easing(completion));
        }
        completion += tickDelta;
    }
    else
    {
        completion = 1;
    }

    afterTick(*this);
    return completion;
}

void Action::transform(Actor& shape, const float& weighting)
{
    switch (transformation)
    {
        case M_DELAY:
        {
            break;
        }
        case M_TRANSLATE:
        {
            const Vec2 pathPosition = Algebra::interpolateSet(path, weighting) + pivot;
            const Vec2 deltaPosition = pathPosition - shape.position;
            shape.position = pathPosition;
            for (auto& vertex : shape.vertices)
                vertex = vertex + deltaPosition;
            break;
        }
        case M_SCALE:
        {
            const Vec2 pathScale = Algebra::interpolateSet(path, weighting);
            shape.scale = pathScale;
            for (size_t i = 0; i < shape.vertices.size(); i++)
                shape.vertices[i] = shape.position + ((shape.localVertices[i] + pivot) * pathScale);
            break;
        }
        case M_ROTATE:
        {
            float interpolatedAngle = Algebra::interpolateSet(path, weighting).x;
            const float deltaAngle = interpolatedAngle - shape.angle.x;
            shape.angle.x = interpolatedAngle;

            float cosTheta = cos(deltaAngle);
            float sinTheta = sin(deltaAngle);

            const Vec2 globalPivot = pivot + shape.position;

            for (auto& localVertex : shape.localVertices) {
                const float dx = localVertex.x;
                const float dy = localVertex.y;

                localVertex.x = (dx * cosTheta - dy * sinTheta);
                localVertex.y = (dx * sinTheta + dy * cosTheta);
            }

            float dx = 0 - pivot.x;
            float dy = 0 - pivot.y;

            shape.position.x += pivot.x + (dx * cosTheta - dy * sinTheta);
            shape.position.y += pivot.y + (dx * sinTheta + dy * cosTheta);

            for (size_t i = 0; i < shape.localVertices.size(); i++)
                shape.vertices[i] = shape.position + shape.localVertices[i];

            pivot = globalPivot - shape.position;

            break;
        }

        default:
            break;
    }
}
