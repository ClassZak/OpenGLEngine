#pragma once
#include <memory>
#include "../../OpenGLClass/Shader.hpp"
#include "IDrawable.hpp"

namespace Zak {
class IHasShader : virtual public IDrawableOpenGL
{
protected:
	std::weak_ptr<Shader> m_shader;
public:
	IHasShader() = default;
	virtual ~IHasShader() = default;

	IHasShader(std::shared_ptr<Shader> shader) :
	m_shader(shader) {}


	std::weak_ptr<Shader> GetShader()
	{
		return m_shader;
	}
	Shader* GetShaderSharedPointer()
	{
		if (auto ptr = m_shader.lock())
			return ptr.get();
		return nullptr;
	}
	void SetShader(std::shared_ptr<Shader> shader)
	{
		m_shader = shader;
	}
};
}