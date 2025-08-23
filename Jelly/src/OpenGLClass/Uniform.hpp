#pragma once
#include <variant>
#include <vector>
#include <array>

// Типы данных униформ
enum class UniformType {
	Int, Float, Bool,
	Vec2, Vec3, Vec4,
	Mat3, Mat4,
	Sampler2D
};

// Информация о типе униформы
struct UniformInfo {
	UniformType type;
	int arraySize = 0; // 0 = не массив
};

// Базовые типы данных
using UniformInt = int;
using UniformFloat = float;
using UniformBool = bool;
using UniformVec2 = glm::vec2;
using UniformVec3 = glm::vec3;
using UniformVec4 = glm::vec4;
using UniformMat3 = glm::mat3;   // row-major
using UniformMat4 = glm::mat4;   // row-major
using UniformSampler = UniformInt;

// Типы для массивов
using UniformIntArray = std::vector<int>;
using UniformFloatArray = std::vector<float>;
using UniformBoolArray = std::vector<bool>;
using UniformVec2Array = std::vector<glm::vec2>;
using UniformVec3Array = std::vector<glm::vec3>;
using UniformVec4Array = std::vector<glm::vec4>;
using UniformMat3Array = std::vector<glm::mat3>;
using UniformMat4Array = std::vector<glm::mat4>;
using UniformSamplerArray = std::vector<int>;

// Вариант для хранения данных
using UniformValue = std::variant
<
	UniformInt, UniformFloat, UniformBool,
	UniformVec2, UniformVec3, UniformVec4,
	UniformMat3, UniformMat4,
	UniformIntArray, UniformFloatArray, UniformBoolArray,
	UniformVec2Array, UniformVec3Array, UniformVec4Array,
	UniformMat3Array, UniformMat4Array,
	UniformSamplerArray
>;


// Дополнительные использования типов
using Uniform1i = UniformInt;










struct Uniform
{
	UniformValue m_value;
	std::string m_name;

	Uniform(const std::string& name, const UniformValue& value) : m_name(name), m_value(value)
	{
	}
};










