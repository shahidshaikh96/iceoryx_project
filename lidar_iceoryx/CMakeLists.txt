cmake_minimum_required(VERSION 3.16)

project(lidar_iceoryx)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use the locally installed iceoryx
list(APPEND CMAKE_PREFIX_PATH
    ${CMAKE_SOURCE_DIR}/../iceoryx_install
)

find_package(iceoryx_platform REQUIRED)
find_package(iceoryx_hoofs REQUIRED)
find_package(iceoryx_posh REQUIRED)

# LiDAR publisher
add_executable(lidar_publisher
    src/lidar_publisher.cpp
)

target_link_libraries(lidar_publisher
    iceoryx_posh::iceoryx_posh
)

# Obstacle detector
add_executable(obstacle_detector
    src/obstacle_detector.cpp
)

target_link_libraries(obstacle_detector
    iceoryx_posh::iceoryx_posh
)
