#include "../include/CustomClasses/Airplane.hpp"


#include <ZakEngine/EngineCore/AssetsManager.hpp>
#include <ZakEngine/EngineCore/Renderer.hpp>




Airplane::Airplane
(
	const float width,
	const float height,
	const std::string &textureName,
	const std::string& shaderName
) : m_width{width}, m_height{height}
{
	Zak::QuadrangleTexture<Zak::Vertex2DText>* image = new Zak::QuadrangleTexture<Zak::Vertex2DText>
	({
		Zak::Vertex2DText({-m_width / 2.f, -m_height / 2.f}, {0,0}),
		Zak::Vertex2DText({-m_width / 2.f,  m_height / 2.f}, {0,1}),
		Zak::Vertex2DText({ m_width / 2.f,  m_height / 2.f}, {1,1}),
		Zak::Vertex2DText({ m_width / 2.f, -m_height / 2.f}, {1,0}),
	});
	image->SetTexture(Zak::AssetsManager::GetInstance().GetTexture(textureName));
	image->SetShader( Zak::AssetsManager::GetInstance().GetShader (shaderName));

	m_planeQuadrangle_ptr = image;
}


Airplane::~Airplane()
{
	if (m_planeQuadrangle_ptr)
		delete m_planeQuadrangle_ptr;
}








void Airplane::Move(const float x, const float y)
{
	for(auto & el : this->m_planeQuadrangle_ptr->GetVertices())
	{
		el.m_pos.x += x;
		el.m_pos.y += y;
	}
	//this->m_planeQuadrangle_ptr->ReBind();
}


void Airplane::Draw()
{
	Zak::Renderer::GetInstance().Draw
	(
		m_planeQuadrangle_ptr,
		Zak::Uniform("u_Color", Zak::UniformVec4(1.f, 1.f, 1.f, 1.f))
	);
}