#include "iceoryx_posh/popo/subscriber.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"

#include "lidar_data.hpp"

#include <cmath>
#include <cstdint>
#include <iostream>

int main()
{
    // Register this process with iceoryx
    iox::runtime::PoshRuntime::initRuntime("ObstacleDetector");

    // Create subscriber
    iox::popo::Subscriber<LidarScan> subscriber(
        {"Lidar", "Sensor", "PointCloud"});

    std::cout << "Obstacle detector started." << std::endl;

    while (true)
    {
        // Try to receive a LiDAR scan
        auto sample = subscriber.take();

        if (sample.has_value())
        {
            float nearest_distance = 100000.0f;

            // Check every point
            for (uint32_t i = 0; i < LidarScan::POINT_COUNT; ++i)
            {
                const auto& point = sample->points[i];

                // Distance from LiDAR origin
                float distance = std::sqrt(
                    point.x * point.x +
                    point.y * point.y +
                    point.z * point.z);

                if (distance < nearest_distance)
                {
                    nearest_distance = distance;
                }
            }

            std::cout << "Received "
                      << LidarScan::POINT_COUNT
                      << " points | "
                      << "Nearest obstacle: "
                      << nearest_distance
                      << " m"
                      << std::endl;

            // Return the sample to iceoryx
            subscriber.release(sample);
        }
    }

    return 0;
}