cmake_minimum_required(VERSION 3.28)
project(ZakOpenGLEngine LANGUAGES CXX)

# 1. Устанавливаем глобальные настройки для всего проекта.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_subdirectory(Engine)
add_subdirectory(Airplanes)
add_subdirectory(Jelly)
add_subdirectory(Graphics)