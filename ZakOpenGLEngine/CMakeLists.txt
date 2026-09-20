cmake_minimum_required(VERSION 3.2...4.4)
cmake_policy(VERSION 3.10)
project(ZakOpenGLEngine LANGUAGES CXX)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "Force minimum policy version")

# 1. Устанавливаем глобальные настройки для всего проекта.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(Engine)
add_subdirectory(Airplanes)
add_subdirectory(Jelly)
add_subdirectory(Graphics)