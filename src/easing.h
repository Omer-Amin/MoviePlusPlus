#pragma once

#include <cmath>
#include <algorithm>

typedef std::function<float(float)> easingFunction;

namespace Easing {

    constexpr float PI = 3.14159265358979323846f;

    static inline float Linear(float x) {
        return x;
    }

    static inline float InQuad(float x) {
        return x * x;
    }

    static inline float OutQuad(float x) {
        return 1.0f - (1.0f - x) * (1.0f - x);
    }

    static inline float InOutQuad(float x) {
        return x < 0.5f ? 2.0f * x * x : 1.0f - powf(-2.0f * x + 2.0f, 2.0f) / 2.0f;
    }

    static inline float InCubic(float x) {
        return x * x * x;
    }

    static inline float OutCubic(float x) {
        return 1.0f - powf(1.0f - x, 3.0f);
    }

    static inline float InOutCubic(float x) {
        return x < 0.5f ? 4.0f * x * x * x : 1.0f - powf(-2.0f * x + 2.0f, 3.0f) / 2.0f;
    }

    static inline float InQuart(float x) {
        return x * x * x * x;
    }

    static inline float OutQuart(float x) {
        return 1.0f - powf(1.0f - x, 4.0f);
    }

    static inline float InOutQuart(float x) {
        return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - powf(-2.0f * x + 2.0f, 4.0f) / 2.0f;
    }

    static inline float InQuint(float x) {
        return x * x * x * x * x;
    }

    static inline float OutQuint(float x) {
        return 1.0f - powf(1.0f - x, 5.0f);
    }

    static inline float InOutQuint(float x) {
        return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - powf(-2.0f * x + 2.0f, 5.0f) / 2.0f;
    }

    static inline float InSine(float x) {
        return 1.0f - cosf((x * PI) / 2.0f);
    }

    static inline float OutSine(float x) {
        return sinf((x * PI) / 2.0f);
    }

    static inline float InOutSine(float x) {
        return -(cosf(PI * x) - 1.0f) / 2.0f;
    }

    static inline float InExpo(float x) {
        return x == 0.0f ? 0.0f : powf(2.0f, 10.0f * x - 10.0f);
    }

    static inline float OutExpo(float x) {
        return x == 1.0f ? 1.0f : 1.0f - powf(2.0f, -10.0f * x);
    }

    static inline float InOutExpo(float x) {
        if (x == 0.0f) return 0.0f;
        if (x == 1.0f) return 1.0f;
        return x < 0.5f ? powf(2.0f, 20.0f * x - 10.0f) / 2.0f : (2.0f - powf(2.0f, -20.0f * x + 10.0f)) / 2.0f;
    }

    static inline float InCirc(float x) {
        return 1.0f - sqrt(1.0f - powf(x, 2.0f));
    }

    static inline float OutCirc(float x) {
        return sqrt(1.0f - powf(x - 1.0f, 2.0f));
    }

    static inline float InOutCirc(float x) {
        return x < 0.5f
            ? (1.0f - sqrt(1.0f - powf(2.0f * x, 2.0f))) / 2.0f
            : (sqrt(1.0f - powf(-2.0f * x + 2.0f, 2.0f)) + 1.0f) / 2.0f;
    }

    static inline float InBack(float x) {
        constexpr float c1 = 1.70158f;
        constexpr float c3 = c1 + 1.0f;
        return c3 * x * x * x - c1 * x * x;
    }

    static inline float OutBack(float x) {
        constexpr float c1 = 1.70158f;
        constexpr float c3 = c1 + 1.0f;
        return 1.0f + c3 * powf(x - 1.0f, 3.0f) + c1 * powf(x - 1.0f, 2.0f);
    }

    static inline float InOutBack(float x) {
        constexpr float c1 = 1.70158f;
        constexpr float c2 = c1 * 1.525f;
        return x < 0.5f
            ? (powf(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2.0f
            : (powf(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
    }

    static inline float InElastic(float x) {
        constexpr float c4 = (2.0f * PI) / 3.0f;
        return x == 0.0f ? 0.0f : x == 1.0f ? 1.0f : -powf(2.0f, 10.0f * x - 10.0f) * sinf((x * 10.0f - 10.75f) * c4);
    }

    static inline float OutElastic(float x) {
        constexpr float c4 = (2.0f * PI) / 3.0f;
        return x == 0.0f ? 0.0f : x == 1.0f ? 1.0f : powf(2.0f, -10.0f * x) * sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
    }

    static inline float InOutElastic(float x) {
        constexpr float c5 = (2.0f * PI) / 4.5f;
        return x == 0.0f ? 0.0f : x == 1.0f ? 1.0f : x < 0.5f
            ? -(powf(2.0f, 20.0f * x - 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f
            : (powf(2.0f, -20.0f * x + 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
    }

    static inline float OutBounce(float x) {
        constexpr float n1 = 7.5625f;
        constexpr float d1 = 2.75f;

        if (x < 1.0f / d1) {
            return n1 * x * x;
        } else if (x < 2.0f / d1) {
            x -= 1.5f / d1;
            return n1 * x * x + 0.75f;
        } else if (x < 2.5f / d1) {
            x -= 2.25f / d1;
            return n1 * x * x + 0.9375f;
        } else {
            x -= 2.625f / d1;
            return n1 * x * x + 0.984375f;
        }
    }

    static inline float InBounce(float x) {
        return 1.0f - OutBounce(1.0f - x);
    }

    static inline float InOutBounce(float x) {
        return x < 0.5f
            ? (1.0f - OutBounce(1.0f - 2.0f * x)) / 2.0f
            : (1.0f + OutBounce(2.0f * x - 1.0f)) / 2.0f;
    }

}