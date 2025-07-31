#pragma once
#include <memory>
#include "../../OpenGLClass/Shader.hpp"
#include "IDrawable.hpp"

class IHasShader : public IDrawableOpenGL
{
protected:
	std::shared_ptr<Shader> m_shader = nullptr;
public:
	IHasShader() = default;
	virtual ~IHasShader() = default;


	std::shared_ptr<Shader> GetShaderSharedPointer()
	{
		return m_shader;
	}
	Shader* GetShader()
	{
		return m_shader.get();
	}
	void SetShader(const std::string& filepath)
	{
		Shader* newShader = new Shader(filepath);
		m_shader.reset(newShader);
	}
};