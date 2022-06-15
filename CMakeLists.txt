cmake_minimum_required(VERSION 3.0.0)
project(BallBalancingRobot VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})

add_executable(BallBalancingRobot main.cpp SerialPort.cpp)

target_link_libraries(BallBalancingRobot ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
