#pragma once

#include "vec2.h"
#include "easing.h"
#include <vector>
#include <functional>

struct Actor;

enum {
        M_TRANSLATE,
        M_SCALE,
        M_ROTATE,
        M_DELAY
    };

struct Action
{
    Action(const Action&) = default;
    Action& operator=(const Action&) = default;

    Action(Action&&) noexcept = default;
    Action& operator=(Action&&) noexcept = default;

    unsigned char transformation;

    std::vector<Vec2> path;
    std::vector<std::function<Vec2(Actor&)>> pathMethod;

    Vec2 pivot;
    std::function<Vec2(Actor&)> pivotMethod;     // local verts

    float duration;
    unsigned int priority = 0;
    unsigned int fps = 60;
    float completion = 0; // 0 to 1
    
    easingFunction easing;

    float frames;
    float tickDelta;

    bool isReversed = false;

    Action(unsigned char transformation, std::vector<std::function<Vec2(Actor&)>> methodForPath, std::function<Vec2(Actor&)> pivotFunc, float duration, std::function<float(float)> easing) : transformation(transformation), duration(duration), easing(easing) {
        pathMethod = methodForPath;
        pivotMethod = pivotFunc;
        frames = fps * duration;
        tickDelta = 1 / frames;
    }

    Action(unsigned char transformation, float duration) : duration(duration) {
        pathMethod = {};
        pivotMethod = [=](Actor& actor){ return Vec2(); };
        frames = fps * duration;
        tickDelta = 1.0f / frames;
        easing = Easing::Linear;
    }


    float tick(Actor&);
    void transform(Actor&, const float&);
    void jumpTo(Actor&, float point = 0);
    void setFPS(unsigned int);
    Action reverse();

    std::function<void(Action&)> afterTick = [&](Action& action) {};

    static std::function<Vec2(Actor&)> arg(float x = 0.0f, float y = 0.0f);
    static std::function<Vec2(Actor&)> arg(Vec2);
    static std::vector<std::function<Vec2(Actor&)>> getPath(std::vector<Vec2>);
};

typedef std::vector<std::vector<Action>> Script;