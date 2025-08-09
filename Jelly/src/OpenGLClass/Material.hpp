#pragma once
#include <list>
#include <map>
#include <string>
#include <memory>
#include <functional>

#include "Shader.hpp"


class Material
{
	std::map<std::string, std::shared_ptr<void*>> m_uniforms;
	std::function<void(void)> m_shaderFunction;
public:
	Material
	(
		const std::map<std::string, std::shared_ptr<void*>>& uniforms,
		const std::function<void(void)>& shaderFunction=nullptr
	)
	{
		m_uniforms.insert(uniforms.begin(), uniforms.end());
		m_shaderFunction = shaderFunction;
	}

	void SetUniform(Shader& shader)
	{
		for (auto& el : m_uniforms)
		{
			if(Uniform_4<float>* unifofm_4f = dynamic_cast<Uniform_4<float>*>(((AUniform*)*el.second.get())))
				shader.SetUniform_4(el.first, *unifofm_4f);
		}
	}
};

