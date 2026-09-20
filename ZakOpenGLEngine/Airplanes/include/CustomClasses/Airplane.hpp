#include <string>


#include <ZakEngine/Shape/Quadrangle.hpp>
#include <ZakEngine/Vertex/Vertex2DText.hpp>




class Airplane {
private:
	Zak::QuadrangleTexture<Zak::Vertex2DText>* m_planeQuadrangle_ptr=nullptr;
	float m_width;
	float m_height;


public:
	Airplane(const float width, const float height, const std::string& textureName, const std::string& shaderName);
	~Airplane();

	void Move(const float x, const float y);
	void Draw();
	void inline ReBind()
	{
		this->m_planeQuadrangle_ptr->ReBind();	
	}
};
