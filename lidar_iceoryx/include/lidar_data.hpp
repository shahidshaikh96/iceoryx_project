#pragma once

#include <cstdint>

struct Point
{
    float x;
    float y;
    float z;
};

struct LidarScan
{
    static constexpr uint32_t POINT_COUNT = 100;

    Point points[POINT_COUNT];
};