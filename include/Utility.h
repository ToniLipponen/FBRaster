#ifndef UTILITY_H
#define UTILITY_H

inline static float clampf(float x, float min, float max)
{
    if(x > max) return max;
    if(x < min) return min;
    return x;
}

inline static int clampi(int x, int min, int max)
{
    if(x > max) return max;
    if(x < min) return min;
    return x;
}


#endif