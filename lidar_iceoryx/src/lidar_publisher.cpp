#include "iceoryx_posh/popo/publisher.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

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

int main()
{
    // Register this process with iceoryx
    iox::runtime::PoshRuntime::initRuntime("LidarPublisher");

    // Create publisher
    iox::popo::Publisher<LidarScan> publisher(
        {"Lidar", "Sensor", "PointCloud"});

    std::cout << "LiDAR publisher started." << std::endl;

    while (true)
    {
        // Allocate memory from iceoryx
        auto sample = publisher.loan();

        if (sample.has_value())
        {
            // Generate fake LiDAR points
            for (uint32_t i = 0; i < LidarScan::POINT_COUNT; ++i)
            {
                (*sample)->points[i].x = static_cast<float>(i) * 0.1f;
                (*sample)->points[i].y = static_cast<float>(i % 20) * 0.05f;
                (*sample)->points[i].z = 0.1f;
            }

            // Send the data
            sample->publish();

            std::cout << "Published "
                      << LidarScan::POINT_COUNT
                      << " points." << std::endl;
        }
        else
        {
            std::cout << "Could not allocate sample." << std::endl;
        }

        // Simulate 10 Hz LiDAR
        std::this_thread::sleep_for(
            std::chrono::milliseconds(100));
    }

    return 0;
}