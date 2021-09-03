cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(IMU_I2C VERSION 1.0)

add_library(imulib STATIC imuLib/i2c_bno055.c)
target_include_directories(imulib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imuLib/include)

add_executable(imu main.cpp)
target_link_libraries(imu PUBLIC imulib)



