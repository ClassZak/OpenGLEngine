#pragma once
#include <memory>
#include "../../OpenGLClass/Texture.hpp"
#include "IDrawable.hpp"

namespace Zak {
class IHasTexture : virtual public IDrawableOpenGL
{
protected:
	std::weak_ptr<Texture> m_texture;
public:
	IHasTexture() = default;
	virtual ~IHasTexture() = default;

	IHasTexture(std::shared_ptr<Texture> texture) : 
	m_texture(texture) {}


	std::weak_ptr<Texture> GetTexture()
	{
		return m_texture;
	}
	Texture* GetTextureSharedPointer()
	{
		if (auto ptr = m_texture.lock())
			return ptr.get();
		return nullptr;
	}
	void SetTexture(std::shared_ptr<Texture> texture)
	{
		m_texture = texture;
	}
};
}