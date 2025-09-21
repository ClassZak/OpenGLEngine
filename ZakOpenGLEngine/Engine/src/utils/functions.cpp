#include "../../include/ZakEngine/utils/functions.hpp"

using namespace Zak;
std::string Zak::LoadDataFromFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	file.seekg(0, file.end);
	int size = file.tellg();
	file.seekg(0, file.beg);
	if (!file.is_open())
	{
		int error;
#ifdef WIN32
		error = GetLastError();
#else
		error = errno;
#endif
		throw error;
	}

	std::string data(size, '\0');
	file.read(data.data(), size);

	return data;
}