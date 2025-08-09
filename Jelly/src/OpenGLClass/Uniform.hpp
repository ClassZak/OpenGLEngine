#pragma once

struct AUniform{};

template<typename T>
struct Uniform_4 : AUniform
{
	T v0, v1, v2, v3;
};