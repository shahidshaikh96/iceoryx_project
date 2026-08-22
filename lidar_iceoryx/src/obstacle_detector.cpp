#include "iceoryx_posh/popo/subscriber.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iox/signal_watcher.hpp"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
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
    iox::runtime::PoshRuntime::initRuntime("ObstacleDetector");

    // Subscribe to the LiDAR point cloud
    iox::popo::Subscriber<LidarScan> subscriber({"Lidar", "Sensor", "PointCloud"});

    std::cout << "Obstacle detector started." << std::endl;

    while (!iox::hasTerminationRequested())
    {
        subscriber
            .take()
            .and_then([](auto& sample) {
                const LidarScan& scan = *sample;

                // Find the closest point to the sensor
                float closestDistance = std::numeric_limits<float>::max();
                for (uint32_t i = 0; i < LidarScan::POINT_COUNT; ++i)
                {
                    const Point& p = scan.points[i];
                    float distance = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
                    if (distance < closestDistance)
                    {
                        closestDistance = distance;
                    }
                }

                if (closestDistance < 1.0F)
                {
                    std::cout << "WARNING: obstacle at " << closestDistance << " m!" << std::endl;
                }
                else
                {
                    std::cout << "Received scan, closest point at " << closestDistance << " m" << std::endl;
                }
            })
            .or_else([](auto& result) {
                if (result != iox::popo::ChunkReceiveResult::NO_CHUNK_AVAILABLE)
                {
                    std::cout << "Error receiving chunk." << std::endl;
                }
            });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
