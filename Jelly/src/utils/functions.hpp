#pragma once
#ifdef WIN32
#include <windows.h>
#elif __unix__
#include <errno.h>
#include <unistd.h>
#endif

#include <string>
#include <fstream>

std::string LoadDataFromFile(const std::string& filename);
