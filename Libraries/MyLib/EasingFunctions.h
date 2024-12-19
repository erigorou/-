    #ifndef EASING_FUNCTIONS_H
    #define EASING_FUNCTIONS_H

    #include "Math.h"
    #include <cmath> // For sin, cos, pow

    // M_PI が定義されていない場合は手動で定義
    #ifndef M_PI
    #define M_PI 3.14159265358979323846
    #endif

    class Easing
    {
    public:
    static float easeInSine(float x) {
        return 1.0f - static_cast<float>(cos((x * M_PI) / 2.0f));
    }

    static float easeOutSine(float x) {
        return static_cast<float>(sin((x * M_PI) / 2.0f));
    }

    static float easeInOutSine(float x) {
        return static_cast<float>(-(cos(M_PI * x) - 1.0) / 2.0);
    }

    static float easeInQuad(float x) {
        return x * x;
    }

    static float easeOutQuad(float x) {
        return 1.0f - (1.0f - x) * (1.0f - x);
    }

    static float easeInOutQuad(float x) {
        return x < 0.5f ? 2.0f * x * x : 1.0f - static_cast<float>(pow(-2.0f * x + 2.0f, 2) / 2.0);
    }

    static float easeInCubic(float x) {
        return x * x * x;
    }

    static float easeOutCubic(float x) {
        return 1.0f - static_cast<float>(pow(1.0f - x, 3));
    }

    static float easeInOutCubic(float x) {
        return x < 0.5f ? 4.0f * x * x * x : 1.0f - static_cast<float>(pow(-2.0f * x + 2.0f, 3) / 2.0);
    }

    static float easeInQuart(float x) {
        return x * x * x * x;
    }

    static float easeOutQuart(float x) {
        return 1.0f - static_cast<float>(pow(1.0f - x, 4));
    }

    static float easeInOutQuart(float x) {
        return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - static_cast<float>(pow(-2.0f * x + 2.0f, 4) / 2.0);
    }

    static float easeInQuint(float x) {
        return x * x * x * x * x;
    }

    static float easeOutQuint(float x) {
        return 1.0f - static_cast<float>(pow(1.0f - x, 5));
    }

    static float easeInOutQuint(float x) {
        return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - static_cast<float>(pow(-2.0f * x + 2.0f, 5) / 2.0);
    }

    static float easeInExpo(float x) {
        return x == 0 ? 0 : static_cast<float>(pow(2.0, 10.0f * x - 10.0f));
    }

    static float easeOutExpo(float x) {
        return x == 1 ? 1 : 1.0f - static_cast<float>(pow(2.0, -10.0f * x));
    }

    static float easeInOutExpo(float x) {
        return x == 0 ? 0 : x == 1 ? 1 : x < 0.5f ? static_cast<float>(pow(2.0, 20.0f * x - 10.0f) / 2.0) : static_cast<float>((2.0f - pow(2.0, -20.0f * x + 10.0f)) / 2.0);
    }

    static float easeInCirc(float x) {
        return 1.0f - static_cast<float>(sqrt(1.0f - pow(x, 2)));
    }

    static float easeOutCirc(float x) {
        return static_cast<float>(sqrt(1.0f - pow(x - 1.0f, 2)));
    }

    static float easeInOutCirc(float x) {
        return x < 0.5f ? static_cast<float>((1.0f - sqrt(1.0f - pow(2.0f * x, 2))) / 2.0) : static_cast<float>((sqrt(1.0f - pow(-2.0f * x + 2.0f, 2)) + 1.0f) / 2.0);
    }

    static float easeInBack(float x) {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return c3 * x * x * x - c1 * x * x;
    }

    static float easeOutBack(float x) {
        const float c1 = 1.70158f;
        const float c3 = c1 + 1.0f;
        return 1.0f + c3 * static_cast<float>(pow(x - 1.0f, 3)) + c1 * static_cast<float>(pow(x - 1.0f, 2));
    }

    static float easeInOutBack(float x) {
        const float c1 = 1.70158f;
        const float c2 = c1 * 1.525f;
        return x < 0.5f ? static_cast<float>((pow(2.0f * x, 2) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2.0) : static_cast<float>((pow(2.0f * x - 2.0f, 2) * ((c2 + 1.0f) * (2.0f * x - 2.0f) + c2) + 2.0f) / 2.0);
    }

    static float easeInElastic(float x) {
        const float c4 = (2.0f * M_PI) / 3.0f;
        return x == 0 ? 0 : x == 1 ? 1 : -static_cast<float>(pow(2.0, 10.0f * x - 10.0f) * sin((x * 10.0f - 10.75f) * c4));
    }

    static float easeOutElastic(float x) {
        const float c4 = (2.0f * M_PI) / 3.0f;
        return x == 0 ? 0 : x == 1 ? 1 : static_cast<float>(pow(2.0, -10.0f * x) * sin((x * 10.0f - 0.75f) * c4) + 1.0f);
    }

    static float easeInOutElastic(float x) {
        const float c5 = (2.0f * M_PI) / 4.5f;
        return x == 0 ? 0 : x == 1 ? 1 : x < 0.5f ? static_cast<float>(-(pow(2.0, 20.0f * x - 10.0f) * sin((20.0f * x - 11.125f) * c5)) / 2.0) : static_cast<float>((pow(2.0, -20.0f * x + 10.0f) * sin((20.0f * x - 11.125f) * c5)) / 2.0 + 1.0f);
    }

    static float easeInBounce(float x) {
        return 1.0f - easeOutBounce(1.0f - x);
    }

    static float easeOutBounce(float x) {
        const float n1 = 7.5625f;
        const float d1 = 2.75f;
        if (x < 1.0f / d1) {
            return n1 * x * x;
        }
        else if (x < 2.0f / d1) {
            return n1 * (x -= 1.5f / d1) * x + 0.75f;
        }
        else if (x < 2.5f / d1) {
            return n1 * (x -= 2.25f / d1) * x + 0.9375f;
        }
        else {
            return n1 * (x -= 2.625f / d1) * x + 0.984375f;
        }
    }

    static float easeInOutBounce(float x) {
	    return x < 0.5
		    ? (1 - easeOutBounce(1 - 2 * x)) / 2
		    : (1 + easeOutBounce(2 * x - 1)) / 2;
    }

 
    static float easeBetweenIn(float t, float threshold, float firstSize, float maxSize)
    {
    // threshold が 0 ~ 1 の範囲外の場合、補正
    threshold = Math::Clamp(threshold, 0.0f, 1.0f);

        if (t <= threshold){
            return firstSize * Easing::easeOutBack(t / threshold);
        }
        else{
            return firstSize + (maxSize - firstSize) * Easing::easeInQuad((t - threshold) / (1.0f - threshold));
        }
    }


    static float easeBetweenOut(float t, float threshold, float firstSize, float maxSize)
    {
        // threshold が 0 ~ 1 の範囲外の場合、補正
        threshold = Math::Clamp(threshold, 0.0f, 1.0f);

        if (t <= threshold)
        {
            return firstSize * Easing::easeOutSine(t / threshold);
        }
        else
        {
            return firstSize + (maxSize - firstSize) * Easing::easeInOutCubic((t - threshold) / (1.0f - threshold));
        }
    }
};

    #endif // EASING_FUNCTIONS_H